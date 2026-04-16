//
// Created by storto on 07/08/2025.
//

#include <Lsp/LanguageServer.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif // _WIN32

#include <filesystem>
namespace fs = std::filesystem;
#include <cstdio>

#include <Lsp/_UTF8StringView.h>
#include <Lsp/_LSP-Glaze.h>

namespace LSP {

const std::unexpected<glz::rpc::error> _LanguageServerImpl::not_initialized_error = std::unexpected(glz::rpc::error(
       static_cast<glz::rpc::error_e>(ErrorCodes::ServerNotInitialized),
       std::nullopt,
       "Server not initialized"
));
const std::expected<_Void, glz::rpc::error> _LanguageServerImpl::none = std::expected<_Void, glz::rpc::error_e>{};

static auto _getItAtPosition(Utf8Iterator it, const Utf8Iterator &end, const Position &pos, const Position &prev = { 0, 0 }) -> Utf8Iterator
{
        for (uinteger line = prev.line; line != pos.line && it != end; ++it) {
                const UTFChar utfChar = *it;
                if (utfChar.kind != UtfCharKind::Utf8)
                        continue;
                const char c = *utfChar.it;
                if (c == '\n') {
                        ++line;
                } else if (c == '\r') {
                        ++line;
                        if (*(utfChar.it + 1) == '\n')  // Safe since std::string
                                ++it;                   // is null-terminated
                }
        }

        uinteger i = pos.line == prev.line ? prev.character : 0;
        for (; i < pos.character && it != end; ++i, ++it)
                if (it->kind > UtfCharKind::Utf16)  // UTF24 and UTF32 use 2 characters
                        ++i;

        return it;
}

static void _applyContentChange(std::string &text, const TextDocumentContentChangeEvent &event)
{
        const auto view = Utf8StringView(text);

        const Utf8Iterator editBegin = _getItAtPosition(view.begin(), view.end(), event.range.start);
        const Utf8Iterator editEnd   = _getItAtPosition(editBegin, view.end(), event.range.end, event.range.start);

        const std::string &repl = std::holds_alternative<std::string>(event.text) ?
                std::get<std::string>(event.text) :
                std::get<TextDocumentContentChangeEvent::_Text>(event.text).text;

        const ptrdiff_t editBeginOffset = editBegin - view.begin();
        const ptrdiff_t editEndOffset   = editEnd - view.begin();
        const size_t    editedSize      = editEndOffset - editBeginOffset;

        text.replace(editBeginOffset, editedSize, repl);
}

void _LanguageServerImpl::setLogger(const std::string &path)
{
        m_log     = true;
        m_logFile = std::ofstream(path, std::ios::binary);
}

auto _LanguageServerImpl::loop() -> int
{
        std::setvbuf(m_in, nullptr, _IONBF, 0);
        std::setvbuf(m_out, nullptr, _IONBF, 0);

#ifdef _WIN32
        _setmode(_fileno(m_in), _O_BINARY);
        _setmode(_fileno(m_out), _O_BINARY);
#endif // _WIN32

        while (not std::feof(m_in)) {
                try {
                        _writeMessage(m_server.call(_readMessage()));
                } catch (ExitSignal &e) {
                        return e.exitCode;
                }
#if !defined(CPP_LSP_API_DISABLE_CATCH) && !(defined(CPP_LSP_API_DISABLE_CATCH_DEBUG) && !defined(NDEBUG))
                catch (std::exception &e) {
                        _writeError(
                                ErrorCodes::RequestFailed,
                                std::string("Internal server error: ") + e.what());
                }
#else // CPP_LSP_API_DISABLE_CATCH || (!CPP_LSP_API_DISABLE_CATCH_DEBUG || NDEBUG)
                catch (std::exception &e) {
                        // The server does not print \n at the end of its messages;
                        // to make the error clear, print 2 new lines.
                        fwrite("\n\n", sizeof(char), 2, m_out);
                        throw;
                }
#endif // CPP_LSP_API_DISABLE_CATCH || (!CPP_LSP_API_DISABLE_CATCH_DEBUG || NDEBUG)
        }

        return EXIT_FAILURE;  // Should always exit due to the "exit"
                              // method, not an EOF.
}

_LanguageServerImpl::_LanguageServerImpl(const ServerInfo &info, const ServerCapabilities &capabilities) :
        m_capabilities(capabilities),
        m_info(info)
{}

auto _LanguageServerImpl::initialize(const InitializeParams &params) -> Expected<InitializeResult>
{
        m_clientCapabilities = params.capabilities;

        return InitializeResult{
                .capabilities = m_capabilities,
                .serverInfo   = m_info
        };
}

auto _LanguageServerImpl::initialized(const InitializedParams &) -> Expected<_Void>
{
        m_initialized = true;
        return none;
}

auto _LanguageServerImpl::shutdown(const _Void &) -> Expected<std::nullptr_t>
{
        m_shutdown = true;
        return nullptr;
}

auto _LanguageServerImpl::exit(const _Void &) -> Expected<_Void>
{
        m_initialized = false;
        if (m_shutdown)
                throw ExitSignal(EXIT_SUCCESS);
        throw ExitSignal(EXIT_FAILURE);
}

void _LanguageServerImpl::_updateTextDocument(std::string &text, const std::vector<TextDocumentContentChangeEvent> &changes)
{
        for (const auto &change : changes)
                _applyContentChange(text, change);
}

auto _LanguageServerImpl::_hierarchyItemToCall(const string &uri, std::vector<HierarchyItem> &items) const -> std::vector<CallHierarchyItem>
{
        namespace rng = std::ranges;
        namespace vws = std::views;

        const auto toLSPElement = [&uri](HierarchyItem &item) -> CallHierarchyItem {
                return CallHierarchyItem{
                        .name           = std::move(item.name),
                        .kind           = item.kind,
                        .tags           = std::move(item.tags),
                        .detail         = item.detail,
                        .uri            = uri,
                        .range          = item.fullRange,
                        .selectionRange = item.range
                };
        };

        return items | vws::transform(toLSPElement) | rng::to<std::vector<CallHierarchyItem>>();
}

auto _LanguageServerImpl::_hierarchyItemToType(const string &uri, std::vector<HierarchyItem> &items) const -> std::vector<TypeHierarchyItem>
{
        namespace rng = std::ranges;
        namespace vws = std::views;

        const auto toLSPElement = [&uri](HierarchyItem &item) -> TypeHierarchyItem {
                return TypeHierarchyItem{
                        .name           = std::move(item.name),
                        .kind           = item.kind,
                        .tags           = std::move(item.tags),
                        .detail         = item.detail,
                        .uri            = uri,
                        .range          = item.fullRange,
                        .selectionRange = item.range
                };
        };

        return items | vws::transform(toLSPElement) | rng::to<std::vector<TypeHierarchyItem>>();
}

auto _LanguageServerImpl::_convertDocumentSymbols(const DocumentUri &uri, std::vector<DocumentSymbol> &symbols) const -> std::vector<WorkspaceSymbol>
{
        std::vector<WorkspaceSymbol> ret;
        ret.reserve(symbols.size());
        for (size_t i = 0; i < symbols.size(); ++i)
                _addWorkspaceSymbol(uri, ret, symbols[i]);

        return ret;
}

auto _LanguageServerImpl::_internalError(const std::string &msg) const -> std::unexpected<glz::rpc::error>
{
        return std::unexpected(glz::rpc::error(
                static_cast<glz::rpc::error_e>(ErrorCodes::RequestFailed),
                std::nullopt,
                msg
        ));
}

auto _LanguageServerImpl::_readHeader() const -> size_t
{
start:
        char check[17]{};
        while (std::fgetc(m_in) != 'C')
                ;

        check[0] = 'C';
        for (uint32_t i = 0; i < 15; ++i)
                check[i + 1] = static_cast<char>(std::fgetc(m_in));

        if (std::strncmp(check, "Content-Length: ", 16) != 0) [[unlikely]]
                goto start;

        char buf[12]{};
        for (int c, i = 0; (c = std::fgetc(m_in)) != '\n' && i < 11; ++i)
                buf[i] = static_cast<char>(c);

        while (std::fgetc(m_in) != '\n')  // Skip empty line (should always be
                ;                         // 2 calls to fgetc)

        return std::stoi(buf);
}

auto _LanguageServerImpl::_readMessage() const -> std::string
{
        const size_t length = _readHeader();
        if (not length)
                throw std::runtime_error("Could not parse message length from header.");

        std::string request{};
        request.resize(length);
        for (size_t pos = 0, read = 0; pos < length; pos += read)
                if (not ((read = std::fread(&request[pos], 1, length - pos, m_in))))
                        break;

        if (m_log)
                m_logFile << "Content-Length: " << length << "\r\n\r\n"
                          << request << "\r\n\r\n" << std::flush;

        return request;
}

void _LanguageServerImpl::_writeMessage(std::string &&msg) const
{
        if (msg.empty() or msg == R"({"jsonrpc":"2.0","result":{"______":"_____"},"id":0})")
                return;

        const std::string header = "Content-Length: " + std::to_string(msg.length()) + "\r\n\r\n";
        const size_t size        = header.length() + msg.length();

        msg.insert(0, header);

        for (size_t pos = 0; pos < size;) {
                const size_t written = std::fwrite(&msg[pos], 1, size, m_out);
                if (written <= 0)
                        break;
                pos += written;
        }

        std::fflush(m_out);
}

void _LanguageServerImpl::_writeError(ErrorCodes error, std::string &&msg) const
{
        _writeMessage(glz::write_json(glz::rpc::error(
                static_cast<glz::rpc::error_e>(error),
                std::nullopt, msg
        )).value());
}

void _LanguageServerImpl::_addWorkspaceSymbol(const DocumentUri &uri, std::vector<WorkspaceSymbol> &symbols, DocumentSymbol &symbol, const std::optional<string> &containerName) const
{
        for (auto &child : symbol.children.value_or({}))
                _addWorkspaceSymbol(uri, symbols, child, symbol.name);

        symbols.push_back({
                .name          = symbol.name,
                .kind          = symbol.kind,
                .tags          = std::move(symbol.tags),
                .containerName = containerName,
                .location      = Location{
                        .uri   = uri,
                        .range = symbol.range
                }
        });
}

} // namespace LSP