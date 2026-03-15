//
// Created by storto on 08/03/2026.
//

#pragma once

#include <glaze/ext/jsonrpc.hpp>
#include <ranges>

#include <LSP-Protocol.h>

namespace LSP {

struct ExitSignal final : std::exception {
        explicit ExitSignal(int exitCode) : exitCode(exitCode) {}
        int exitCode;

}; // struct ExitSignal

template<typename T, typename V>
concept PtrLike = std::same_as<T, const V *> or requires(T v)
{
        { !v } -> std::same_as<bool>;
        { v.operator->() } -> std::same_as<const V *>;

};

template<typename T>
concept File = requires(const T v)
{
        requires std::same_as<T, std::remove_cvref_t<T>>;

        typename T::TokenT;
        requires std::same_as<typename T::TokenT, std::remove_cvref_t<typename T::TokenT>>;

        typename T::TokenItT;
        typename T::TokenPtrT;

        { v.begin() } -> std::same_as<typename T::TokenItT>;
        { v.end() } -> std::same_as<typename T::TokenItT>;
        { T::parse(std::string{}) } -> std::same_as<std::unique_ptr<T>>;

}
and requires (typename T::TokenItT it)
{
        ++it;
        { it == it } -> std::same_as<bool>;
        { it != it } -> std::same_as<bool>;
        requires std::same_as<std::remove_cvref_t<decltype(*it)>, typename T::TokenT>;
        requires std::is_reference_v<decltype(*it)>;

}
and requires (typename T::TokenPtrT p)
{
        requires PtrLike<typename T::TokenPtrT, typename T::TokenT>;
}
and requires (const typename T::TokenT t)
{
        { t.range() } -> std::same_as<Range>;
        { t.operator<=>(t) } -> std::same_as<std::weak_ordering>;
        { t.operator<=>(Position{}) } -> std::same_as<std::weak_ordering>;

}; // concept File

template<typename T>
concept Notebook = requires(T v)
{
        requires File<T>;
        { T::notebook_selector } -> std::same_as<NotebookDocumentSyncOptions::_NotebookSelector>;
        { T::parse(std::string{}, NotebookCellKind{}, std::vector<T *>{}) } -> std::same_as<std::unique_ptr<T>>;

}; // concept Notebook

template<typename T>
concept HasEditsOnSave = requires(const T v)
{
        requires File<T>;
        { v.save() } -> std::convertible_to<std::vector<TextEdit>>;

}; // concept HasResolve

template<typename T>
concept HasResolve = requires(const T v)
{
        requires File<T>;
        { v.resolve(Position{}) } -> std::same_as<typename T::TokenPtrT>;

}; // concept HasResolve

template<typename T>
concept HasReferences = requires(const T v)
{
        requires File<T>;
        { v.references(Position{}) } -> std::convertible_to<std::vector<typename T::TokenPtrT>>;

}; // concept HasReferences

template<typename T>
concept HasSemanticTokens = requires(const T v)
{
        requires File<T>;
        { T::token_types } -> std::convertible_to<std::vector<string>>;
        { T::token_modifiers } -> std::convertible_to<std::vector<string>>;

}
and requires (const typename T::TokenT t)
{
        { static_cast<uinteger>(T::TokenT::invalid_type) };
        { static_cast<uinteger>(t.type()) };
        { static_cast<uinteger>(t.modifier()) };

}; // concept HasSemanticTokens

template<typename T>
concept HasHover = requires(const T v)
{
        requires File<T>;
        { v.hover(Position{}) } -> std::same_as<std::optional<MarkupContent>>;

}; // concept HasHover

template<typename T>
concept HasCodeLens = requires(const T v)
{
        requires File<T>;
        { v.codeLenses() } -> std::convertible_to<std::vector<CodeLens>>;

}; // concept HasCodeLens

template<typename T>
concept HasDiagnostic = requires(const T v)
{
        requires File<T>;
        { T::diagnostic_internal_dependencies } -> std::convertible_to<bool>;
        { v.diagnostics() } -> std::convertible_to<std::vector<Diagnostic>>;

}; // concept HasDiagnostic

struct HierarchyItem {
        string                 name;
        string                 detail;
        std::vector<SymbolTag> tags;
        Range                  fullRange;
        Range                  range;
        SymbolKind             kind;
        std::vector<Range>     sources;  // Only for functions:
                                         // in parent items, where the element is
                                         // referred to, in children where the
                                         // element refers to them.

}; // struct HierarchyItem

template<typename T>
concept HasHierarchy = requires(T v)
{
        requires File<T>;
        { v.roots(Position{}) } -> std::convertible_to<std::vector<HierarchyItem>>;
        { v.children(std::string{}, Range{}) } -> std::convertible_to<std::vector<HierarchyItem>>;
        { v.parents(std::string{}, Range{}) } -> std::convertible_to<std::vector<HierarchyItem>>;

}; // concept HasHierarchy

template<typename T>
concept HasHighlight = requires(T v)
{
        requires File<T>;
        { v.highlights(Position{}) } -> std::convertible_to<std::vector<DocumentHighlight>>;

}; // concept HasHighlight

struct Link {
        Range                 range;
        URI                   target;
        std::optional<string> tooltip;

}; // struct Link

template<typename T>
concept HasLinks = requires(T v)
{
        requires File<T>;
        { v.links() } -> std::convertible_to<std::vector<Link>>;

}; // concept HasLinks

struct Fold {
        Range                 range;
        FoldingRangeKind      kind;
        std::optional<string> preview;

}; // struct Fold

template<typename T>
concept HasFoldingRange = requires(T v)
{
        requires File<T>;
        { v.foldingRanges() } -> std::convertible_to<std::vector<Fold>>;

}; // concept HasFoldingRange

template<typename T>
concept HasSelectionRange = requires(T v)
{
        requires File<T>;
        // Returned values are increasingly big.
        { v.selectionRanges(Position{}) } -> std::convertible_to<std::vector<Range>>;

}; // concept HasSelectionRange

template<typename T>
concept HasSymbols = requires(T v)
{
        requires File<T>;
        { v.symbols() } -> std::convertible_to<std::vector<DocumentSymbol>>;

}; // concept HasSymbols

template<typename T>
concept HasInlayHints = requires(T v)
{
        requires File<T>;
        { v.hints(Range{}) } -> std::convertible_to<std::vector<InlayHint>>;

}; // concept HasInlayHints

template<typename T>
concept HasInlineValue = requires(T v)
{
        requires File<T>;
        { v.inlineValues(Range{}, InlineValueContext{}) } -> std::convertible_to<std::vector<InlineValue>>;

}; // concept HasInlineValue

template<typename T>
concept HasCompletion = requires(T v)
{
        requires File<T>;
        { v.completions(Position{}) } -> std::convertible_to<std::vector<CompletionItem>>;

}; // concept HasCompletion

template<typename T>
concept HasSignatureHelp = requires(const T v)
{
        requires File<T>;
        { v.signatureHelp(Position{}) } -> std::same_as<std::optional<SignatureHelp>>;

}; // concept HasSignatureHelp

template<typename T>
concept HasCodeAction = requires(const T v)
{
        requires File<T>;
        { v.actions(Range{}, std::optional<CodeActionKind>{}) } -> std::convertible_to<std::vector<std::variant<Command, CodeAction>>>;

}; // concept HasCodeAction

template<typename T>
concept HasColors = requires(const T v)
{
        requires File<T>;
        { v.colors() } -> std::convertible_to<std::vector<ColorInformation>>;
        { v.placeColor(Color{}, Range{}) } -> std::convertible_to<std::vector<ColorPresentation>>;

}; // concept HasColors

template<typename T>
concept HasFormatting = requires(const T v)
{
        requires File<T>;
        { v.format(FormattingOptions{}) } -> std::convertible_to<std::vector<TextEdit>>;
        { v.format(FormattingOptions{}, Range{}) } -> std::convertible_to<std::vector<TextEdit>>;

}; // concept HasFormatting

template<typename T>
concept HasOnTypeFormatting = requires(const T v)
{
        requires HasFormatting<T>;
        { T::on_type_trigger_chars } -> std::convertible_to<std::vector<string>>;
        { v.format(FormattingOptions{}, Position{}) } -> std::convertible_to<std::vector<TextEdit>>;

}; // concept HasOnTypeFormatting

template<typename T>
concept HasRename = requires(const T v)
{
        requires File<T>;

        typename T::RefactorContextT;
        requires std::same_as<std::remove_cvref_t<typename T::RefactorContextT>, typename T::RefactorContextT>;

        { v.createRenameCtx(Position{}, string{}) } -> std::same_as<typename T::RefactorContextT>;
        { v.rename(typename T::RefactorContextT{}) } -> std::convertible_to<std::vector<TextEdit>>;

}; // concept HasRename

template<typename T>
concept HasPrepareRename = requires(const T v)
{
        requires HasRename<T>;
        { v.canRename(Position{}) } -> std::same_as<std::expected<Range, string>>;

}; // concept HasPrepareRename

template<typename T>
concept HasLinkedEditing = requires(const T v)
{
        requires HasRename<T>;
        { v.editingRanges(Position{}) } -> std::same_as<LinkedEditingRanges>;

}; // concept HasLinkedEditing

template<typename T>
concept HasInlineCompletion = requires(const T v)
{
        requires HasRename<T>;
        { v.inlineCompletions(Position{}) } -> std::convertible_to<std::vector<InlineCompletionItem>>;

}; // concept HasInlineCompletion

class _LanguageServerImpl {
        using ServerT = glz::rpc::server<
                InitializeRequest,
                InitializedNotification,
                SetTraceNotification,
                LogTraceNotification,
                ShutdownRequest,
                ExitNotification,
                DidOpenTextDocumentNotification,
                DidChangeTextDocumentNotification,
                WillSaveTextDocument,
                WillSaveWaitUntilTextDocumentRequest,
                DidSaveTextDocumentNotification,
                DidCloseTextDocumentNotification,
                DidOpenNotebookDocumentNotification,
                DidChangeNotebookDocumentNotification,
                DidSaveNotebookDocumentNotification,
                DidCloseNotebookDocumentNotification,
                GotoDeclarationRequest,
                GotoDefinitionRequest,
                GotoTypeDefinitionRequest,
                GotoImplementationRequest,
                FindReferencesRequest,
                PrepareCallHierarchyRequest,
                CallHierarchyIncomingCalls,
                CallHierarchyOutgoingCalls,
                PrepareTypeHierarchyRequest,
                TypeHierarchySupertypes,
                TypeHierarchySubtypes,
                DocumentHighlightsRequest,
                DocumentLinkRequest,
                DocumentLinkResolveRequest,
                HoverRequest,
                CodeLensRequest,
                CodeLensResolveRequest,
                FoldingRangeRequest,
                SelectionRangeRequest,
                DocumentSymbolsRequest,
                SemanticTokensFullRequest,
                SemanticTokensDeltaFullRequest,
                SemanticTokensRangeRequest,
                InlayHintRequest,
                InlayHintResolveRequest,
                InlineValueRequest,
                Monikers,
                CompletionRequest,
                CompletionItemResolveRequest,
                DocumentDiagnostics,
                WorkspaceDiagnostics,
                DiagnosticsRefresh,
                SignatureHelpRequest,
                CodeActionRequest,
                CodeActionResolveRequest,
                DocumentColorRequest,
                ColorPresentationRequest,
                DocumentFormattingRequest,
                DocumentRangeFormattingRequest,
                DocumentRangesFormattingRequest,
                DocumentOnTypeFormattingRequest,
                RenameRequest,
                PrepareRenameRequest,
                LinkedEditingRange,
                InlineCompletionRequest,
                WorkspaceSymbolsRequest,
                WorkspaceSymbolResolveRequest,
                DidChangeConfigurationNotification,
                DidChangeWorkspaceFoldersNotification,
                WillCreateFilesRequest,
                DidCreateFilesNotification,
                WillRenameFilesRequest,
                DidRenameFilesNotification,
                WillDeleteFilesRequest,
                DidDeleteFilesNotification,
                DidChangeWatchedFilesNotification,
                ExecuteCommand,
                TextDocumentContentRequest,
                CancelWorkDoneProgress
        >;
        using ClientT = glz::rpc::client<
                RegisterCapability,
                UnregisterCapability,
                CodeLensRefreshRequest,
                FoldingRangeRefreshRequest,
                SemanticTokensRefreshRequest,
                InlayHintRefreshRequest,
                InlineValueRefreshRequest,
                PublishDiagnosticsNotification,
                ConfigurationRequest,
                WorkspaceFoldersRequest,
                ApplyEdit,
                TextDocumentContentRefreshRequest,
                ShowMessageNotification,
                ShowMessageRequest,
                ShowDocumentRequest,
                LogMessageNotification,
                CreateWorkDoneProgress,
                TelemetryNotification
        >;

public:
        void setLogger(const std::string &path);
        auto loop() -> int;

protected:
        template<class T>
        using Expected = std::expected<T, glz::rpc::error>;

        static const std::unexpected<glz::rpc::error>      not_initialized_error;
        static const std::unexpected<glz::rpc::error>      internal_error;
        static const std::expected<_Void, glz::rpc::error> none;

        ServerT m_server;  // TODO: use glz::rpc::registry
        ClientT m_client;  // The server itself can send requests to the client
                           // TODO: make an API to send requests to the client

        ServerCapabilities m_capabilities;
        ServerInfo         m_info;
        ClientCapabilities m_clientCapabilities;
        bool               m_initialized = false;
        bool               m_shutdown    = false;

        mutable std::ofstream m_logFile;
        bool                  m_log = false;

        // TODO: allow use of sockets
        FILE *m_in  = stdin;
        FILE *m_out = stdout;

        _LanguageServerImpl(const ServerInfo &info, const ServerCapabilities &capabilities);

        void _updateTextDocument(std::string &text, const std::vector<TextDocumentContentChangeEvent> &changes);
        auto _hierarchyItemToCall(const string &uri, std::vector<HierarchyItem> &items) const -> std::vector<CallHierarchyItem>;
        auto _hierarchyItemToType(const string &uri, std::vector<HierarchyItem> &items) const -> std::vector<TypeHierarchyItem>;
        auto _convertDocumentSymbols(const DocumentUri &uri, std::vector<DocumentSymbol> &symbols) const -> std::vector<WorkspaceSymbol>;

        auto initialize(const InitializeParams &params) -> Expected<InitializeResult>;
        auto initialized(const InitializedParams &params) -> Expected<_Void>;
        auto shutdown(const _Void &params) -> Expected<std::nullptr_t>;
        auto exit(const _Void &params) -> Expected<_Void>;

private:
        auto _readHeader() const -> size_t;
        auto _readMessage() const -> std::string;
        void _writeMessage(std::string &&msg) const;
        void _writeError(ErrorCodes error, std::string &&msg) const;
        void _addWorkspaceSymbol(const DocumentUri &uri, std::vector<WorkspaceSymbol> &symbols, const DocumentSymbol &symbol, const std::optional<string> &containerName = std::nullopt) const;

}; // class _LanguageServerImpl

template<File FileT>
class LanguageServer : public _LanguageServerImpl
{
        // TODO: move as much code as possible to .cpp
        //  file for faster compilations

        using BaseT     = _LanguageServerImpl;
        using TokenT    = FileT::TokenT;
        using TokenItT  = FileT::TokenItT;
        using TokenPtrT = FileT::TokenPtrT;

public:
        LanguageServer(const ServerInfo &info) : BaseT(info, _getCapabilities())
        {
#define _listen(__method__, __function__) m_server.on<__method__>([this](const auto &params) { return this->__function__(params); })

                _listen("initialize", initialize);
                _listen("initialized", initialized);

                // TODO: $/setTrace and $/logTrace

                _listen("shutdown", shutdown);
                _listen("exit", exit);

                _listen("textDocument/didOpen", didOpen);
                _listen("textDocument/didChange", didChange);

                _listen("textDocument/willSave", willSave);
                if constexpr (HasEditsOnSave<FileT>)
                        _listen("textDocument/willSaveWaitUntil", willSaveWaitUntil);

                _listen("textDocument/didSave", didSave);
                _listen("textDocument/didClose", didClose);

                if constexpr (Notebook<FileT>) {
                        _listen("notebookDocument/didOpen", didOpenNotebook);
                        _listen("notebookDocument/didChange", didChangeNotebook);
                        _listen("notebookDocument/didSave", didSaveNotebook);
                        _listen("notebookDocument/didClose", didCloseNotebook);
                }

                if constexpr (HasResolve<FileT>) {
                        _listen("textDocument/declaration", declaration);
                        _listen("textDocument/definition", definition);
                        _listen("textDocument/typeDefinition", typeDefinition);
                        _listen("textDocument/implementation", implementation);
                }

                if constexpr (HasReferences<FileT>)
                        _listen("textDocument/references", references);

                if constexpr (HasHierarchy<FileT>) {
                        _listen("textDocument/prepareCallHierarchy", prepareCallHierarchy);
                        _listen("callHierarchy/incomingCalls", incomingCalls);
                        _listen("callHierarchy/outgoingCalls", outgoingCalls);
                        _listen("textDocument/prepareTypeHierarchy", prepareTypeHierarchy);
                        _listen("textDocument/supertypes", superTypes);
                        _listen("textDocument/subtypes", subTypes);
                }

                if constexpr (HasHighlight<FileT>)
                        _listen("textDocument/documentHighlight", documentHighlight);

                if constexpr (HasLinks<FileT>) {
                        _listen("textDocument/documentLink", documentLink);
                        // TODO: documentLink/resolve
                }

                if constexpr (HasHover<FileT>)
                        _listen("textDocument/hover", hover);

                if constexpr (HasCodeLens<FileT>) {
                        _listen("textDocument/codeLens", codeLens);
                        // TODO: codeLens/resolve
                }

                if constexpr (HasFoldingRange<FileT>)
                        _listen("textDocument/foldingRange", foldingRange);

                if constexpr (HasSelectionRange<FileT>)
                        _listen("textDocument/selectionRange", selectionRange);

                if constexpr (HasSymbols<FileT>) {
                        _listen("textDocument/documentSymbol", documentSymbol);
                        _listen("workspace/symbol", workspaceSymbol);
                        // TODO: workspaceSymbol/resolve
                }

                if constexpr (HasSemanticTokens<FileT>) {
                        _listen("textDocument/semanticTokens/full", semanticTokensFull);
                        // WILL NOT IMPLEMENT: textDocument/semanticTokens/full/delta
                        _listen("textDocument/semanticTokens/range", semanticTokensRange);
                }

                if constexpr (HasInlayHints<FileT>) {
                        _listen("textDocument/inlayHint", inlayHint);
                        // TODO: inlayHint/resolve
                }

                if constexpr (HasInlineValue<FileT>)
                        _listen("textDocument/inlineValue", inlayValue);

                // TODO: textDocument/moniker

                if constexpr (HasCompletion<FileT>) {
                        _listen("textDocument/completion", completion);
                        // TODO: completionItem/resolve
                }

                if constexpr (HasDiagnostic<FileT>) {
                        _listen("textDocument/diagnostic", diagnostic);
                        _listen("workspace/diagnostic", workspaceDiagnostic);
                }

                if constexpr (HasSignatureHelp<FileT>)
                        _listen("textDocument/signatureHelp", signatureHelp);

                if constexpr (HasCodeAction<FileT>) {
                        _listen("textDocument/codeAction", codeAction);
                        // WILL NOT IMPLEMENT: codeAction/resolve
                }

                if constexpr (HasColors<FileT>) {
                        _listen("textDocument/documentColor", documentColor);
                        _listen("textDocument/colorPresentation", colorPresentation);
                }

                if constexpr (HasFormatting<FileT>) {
                        _listen("textDocument/formatting", format);
                        _listen("textDocument/rangeFormatting", formatRange);
                        _listen("textDocument/rangesFormatting", formatRanges);
                }

                if constexpr (HasOnTypeFormatting<FileT>)
                        _listen("textDocument/onTypeFormatting", onTypeFormat);

                if constexpr (HasRename<FileT>)
                        _listen("textDocument/rename", rename);

                if constexpr (HasPrepareRename<FileT>)
                        _listen("textDocument/prepareRename", checkRename);

                if constexpr (HasLinkedEditing<FileT>)
                        _listen("textDocument/linkedEditingRange", linkedEditing);

                if constexpr (HasInlineCompletion<FileT>)
                        _listen("textDocument/inlineCompletion", inlineCompletion);

                // TODO: workspace/didChangeConfiguration
                // TODO: workspace/didChangeWorkspaceFolders
                // TODO: workspace/willCreateFiles
                // TODO: workspace/didCreateFiles
                // TODO: workspace/willRenameFiles
                // TODO: workspace/didRenameFiles
                // TODO: workspace/willDeleteFiles
                // TODO: workspace/didDeleteFiles
                // TODO: workspace/textDocumentContent
                // TODO: workspace/executeCommand
                // TODO: window/workDoneProgress/cancel
#undef _listen
        }

private:
        struct FileWrapper {
                TextDocumentItem       textDocument;
                std::unique_ptr<FileT> file;

                FileWrapper(const TextDocumentItem &document, std::unique_ptr<FileT> &&file) :
                        textDocument(document),
                        file(std::move(file))
                {}

                FileWrapper(const FileWrapper &) = delete;
                FileWrapper &operator= (const FileWrapper &) = delete;

        }; // struct FileWrapper

        struct NotebookWrapper {
                NotebookDocument       notebookDocument;
                std::list<DocumentUri> cells;

                NotebookWrapper(NotebookDocument &&document, std::list<DocumentUri> &&cells) :
                        notebookDocument(std::move(document)),
                        cells(std::move(cells))
                {}

                NotebookWrapper(const NotebookWrapper &) = delete;
                NotebookWrapper &operator= (const NotebookWrapper &) = delete;

        }; // struct NotebookWrapper

        std::unordered_map<DocumentUri, FileWrapper>     m_openDocuments;
        std::unordered_map<DocumentUri, NotebookWrapper> m_openNotebooks;

        auto didOpen(const DidOpenTextDocumentParams &params) -> Expected<_Void>
        {
                if (not m_initialized)
                        return not_initialized_error;

                m_openDocuments.try_emplace(
                        params.textDocument.uri,
                        params.textDocument,
                        FileT::parse(params.textDocument.text)
                );

                return none;
        }

        auto didChange(const DidChangeTextDocumentParams &params) -> Expected<_Void>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                TextDocumentItem &doc = it->second.textDocument;
                if (doc.version >= params.textDocument.version)
                        return none;

                _updateTextDocument(doc.text, params.contentChanges);

                doc.version     = params.textDocument.version;
                it->second.file = FileT::parse(doc.text);

                return none;
        }

        auto willSave(const WillSaveTextDocumentParams &params) -> Expected<_Void>
        {
                if (not m_initialized)
                        return not_initialized_error;

                (void)params;
                return none;
        }

        auto willSaveWaitUntil(const WillSaveTextDocumentParams &params) -> Expected<std::variant<null, std::vector<TextEdit>>>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<TextEdit> edits = it->second.file->save();
                if (edits.empty())
                        return nullptr;

                return edits;
        }

        auto didSave(const DidSaveTextDocumentParams &) -> Expected<_Void>
        {
                if (not m_initialized)
                        return not_initialized_error;

                return none;
        }

        auto didClose(const DidCloseTextDocumentParams &params) -> Expected<_Void>
        {
                if (not m_initialized)
                        return not_initialized_error;

                m_openDocuments.erase(params.textDocument.uri);
                return none;
        }

        auto declaration(const DeclarationParams &params) -> Expected<std::variant<null, Location, std::vector<Location>, std::vector<LocationLink>>>
                requires HasResolve<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                const std::unique_ptr<FileT> &file = it->second.file;

                TokenPtrT resolved = file->resolve(params.position);
                if (not resolved)
                        return nullptr;

                return Location{
                        .uri   = params.textDocument.uri,
                        .range = resolved->range()
                };
        }

        auto definition(const DefinitionParams &params) -> Expected<std::variant<null, Location, std::vector<Location>, std::vector<LocationLink>>>
                requires HasResolve<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                return this->declaration(reinterpret_cast<const DeclarationParams &>(params));
        }

        auto typeDefinition(const TypeDefinitionParams &params) -> Expected<std::variant<null, Location, std::vector<Location>, std::vector<LocationLink>>>
                requires HasResolve<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                return this->declaration(reinterpret_cast<const DeclarationParams &>(params));
        }

        auto implementation(const ImplementationParams &params) -> Expected<std::variant<null, Location, std::vector<Location>, std::vector<LocationLink>>>
                requires HasResolve<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                return this->declaration(reinterpret_cast<const DeclarationParams &>(params));
        }

        auto references(const ReferenceParams &params) -> Expected<std::variant<null, std::vector<Location>>>
                requires HasReferences<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                const std::unique_ptr<FileT> &file = it->second.file;

                std::vector<Location> locations;
                for (TokenPtrT ref : file->references(params.position))
                        locations.push_back({
                                .uri   = params.textDocument.uri,
                                .range = ref->range()
                        });

                if (locations.empty())
                        return nullptr;

                return locations;
        }

        auto hover(const HoverParams &params) -> Expected<std::variant<null, Hover>>
                requires HasHover<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                const std::optional<MarkupContent> hover = it->second.file->hover();
                if (not hover.has_value())
                        return nullptr;

                return Hover{
                        .contents = hover.value()
                };
        }

        auto codeLens(const CodeLensParams &params) -> Expected<std::variant<null, std::vector<CodeLens>>>
                requires HasCodeLens<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<CodeLens> lenses = it->second.file->codeLenses();
                if (lenses.empty())
                        return nullptr;

                return lenses;
        }

        auto didOpenNotebook(const DidOpenNotebookDocumentParams &params) -> Expected<_Void>
                requires Notebook<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                std::vector<DocumentUri> cells;
                std::list<FileT *> previous;
                cells.resize(params.notebookDocument.cells.size());
                for (size_t i = 0; i < params.notebookDocument.cells.size(); ++i) {
                        const NotebookCell &cell = params.notebookDocument.cells[i];
                        cells[i]                 = cell.document;

                        auto it = params.cellTextDocuments.begin() + static_cast<ptrdiff_t>(i);
                        if (it->uri != cell.document)  // Assume ordered, fallback if necessary
                                it = rng::find_if(params.cellTextDocuments, [uri = cell.document](const TextDocumentItem &text) -> bool { return text.uri == uri; });

                        if (auto insertion = m_openDocuments.try_emplace(
                                cell.document,
                                *it,
                                FileT::parse(it->text, cell.kind, previous));
                            insertion.second)
                                previous.push_back(insertion.first->second.get());
                }

                m_openNotebooks.try_emplace(
                        params.notebookDocument.uri,
                        std::move(params.notebookDocument),
                        std::move(cells));

                return none;
        }

        auto didChangeNotebook(const DidChangeNotebookDocumentParams &params) -> Expected<_Void>
                requires Notebook<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openNotebooks.find(params.notebookDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                NotebookWrapper &notebook = it->second;
                if (not params.change.cells.has_value()
                    or notebook.notebookDocument.version >= params.notebookDocument.version)
                        return none;

                for (const auto &change : params.change.cells->textContent.value_or({})) {
                        const auto docIt = m_openDocuments.find(change.document);
                        if (docIt == m_openDocuments.end())
                                return internal_error;

                        TextDocumentItem &doc = docIt->second.textDocument;
                        _updateTextDocument(doc.text, change.changes);

                        doc.version        = change.document.version;
                        docIt->second.file = FileT::parse(doc.text);
                }

                if (params.change.cells->structure.has_value()) {
                        const auto &structure = params.change.cells->structure.value();
                        auto startIt          = notebook.cells.begin();
                        for (uinteger i = 0; i < structure.array.start; ++i)
                                ++startIt;

                        for (uinteger i = 0; i < structure.array.deleteCount; ++i)
                                if (auto next = std::next(startIt); next != notebook.cells.end()) [[unlikely]]
                                        notebook.cells.erase();

                        if (structure.array.cells.has_value()
                            and structure.didOpen.has_value()) {
                                ++startIt;  // insert() puts before the iterator

                                std::vector<FileT *> previous(structure.array.start);
                                for (uinteger i = 0; i < structure.array.start; ++i) {
                                        auto docIt = m_openDocuments.find(notebook.cells[i]);
                                        if (docIt == m_openDocuments.end())
                                                return internal_error;

                                        previous[i] = docIt->second.file->get();
                                }

                                for (auto &cell : structure.array.cells.value()) {
                                        notebook.cells.insert(startIt, cell.document);

                                        const auto docIt = rng::find_if(
                                                structure.didOpen.value(),
                                                [uri = cell.document](const TextDocumentItem &document) -> bool { return document.uri == uri; });
                                        if (docIt == structure.didOpen.value().end())
                                                return internal_error;

                                        const auto added = m_openDocuments.try_emplace(
                                                docIt->uri,
                                                *docIt,
                                                FileT::parse(docIt->text, cell.kind, previous));
                                        if (added.first)
                                                previous.push_back(added.second->second.file->get());
                                }
                        }

                        for (const auto &id : structure.didClose.value_or({}))
                                m_openDocuments.erase(id.uri);
                }

                notebook.notebookDocument.version = params.notebookDocument.version;
                return none;
        }

        auto didSaveNotebook(const DidSaveNotebookDocumentParams &) -> Expected<_Void>
                requires Notebook<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                return none;
        }

        auto didCloseNotebook(const DidCloseNotebookDocumentParams &params) -> Expected<_Void>
                requires Notebook<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.notebookDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                for (const auto &cell : it->second.cells.value())
                        m_openDocuments.erase(cell.uri);

                m_openNotebooks.erase(it);
                return none;
        }

        auto prepareCallHierarchy(const CallHierarchyPrepareParams &params) -> Expected<std::variant<null, std::vector<CallHierarchyItem>>>
                requires HasHierarchy<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<HierarchyItem> roots = it->second.file->roots();
                if (roots.empty())
                        return nullptr;

                return _hierarchyItemToCall(params.textDocument.uri, roots);
        }

        auto incomingCalls(const CallHierarchyIncomingCallsParams &params) -> Expected<std::variant<null, std::vector<CallHierarchyIncomingCall>>>
                requires HasHierarchy<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.item.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<HierarchyItem> parents = it->second.file->parents(
                        params.item.name, params.item.selectionRange);
                if (parents.empty())
                        return nullptr;

                auto converted = _hierarchyItemToCall(params.item.uri, parents);
                std::vector<CallHierarchyIncomingCall> ret(converted.size());
                for (size_t i = 0; i < ret.size(); ++i) {
                        ret[i] = {
                                .from       = std::move(converted[i]),
                                .fromRanges = std::move(parents[i].sources)
                        };
                }

                return ret;
        }

        auto outgoingCalls(const CallHierarchyOutgoingCallsParams &params) -> Expected<std::variant<null, std::vector<CallHierarchyOutgoingCall>>>
                requires HasHierarchy<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.item.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<HierarchyItem> children = it->second.file->children(
                        params.item.name, params.item.selectionRange);
                if (children.empty())
                        return nullptr;

                auto converted = _hierarchyItemToCall(params.item.uri, children);
                std::vector<CallHierarchyOutgoingCall> ret(converted.size());
                for (size_t i = 0; i < ret.size(); ++i) {
                        ret[i] = {
                                .to         = std::move(converted[i]),
                                .fromRanges = std::move(children[i].sources)
                        };
                }

                return ret;
        }

        auto prepareTypeHierarchy(const TypeHierarchyPrepareParams &params) -> Expected<std::variant<null, std::vector<TypeHierarchyItem>>>
                requires HasHierarchy<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<HierarchyItem> roots = it->second.file->roots();
                if (roots.empty())
                        return nullptr;

                return _hierarchyItemToType(params.textDocument.uri, roots);
        }

        auto superTypes(const TypeHierarchySupertypesParams &params) -> Expected<std::variant<null, std::vector<TypeHierarchyItem>>>
                requires HasHierarchy<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.item.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<HierarchyItem> parents = it->second.file->parents(
                        params.item.name, params.item.selectionRange);
                if (parents.empty())
                        return nullptr;

                return _hierarchyItemToType(params.item.uri, parents);
        }

        auto subTypes(const TypeHierarchySubtypesParams &params) -> Expected<std::variant<null, std::vector<TypeHierarchyItem>>>
                requires HasHierarchy<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.item.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<HierarchyItem> children = it->second.file->children(
                        params.item.name, params.item.selectionRange);
                if (children.empty())
                        return nullptr;

                return _hierarchyItemToType(params.item.uri, children);
        }

        auto documentHighlight(const DocumentHighlightParams &params) -> Expected<std::variant<null, std::vector<DocumentHighlight>>>
                requires HasHighlight<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<DocumentHighlight> highlights = it->second.file->highlights();
                if (highlights.empty())
                        return nullptr;

                return highlights;
        }

        auto documentLink(const DocumentLinkParams &params) -> Expected<std::variant<null, std::vector<DocumentLink>>>
                requires HasLinks<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<Link> links = it->second.file->links();
                if (links.empty())
                        return nullptr;

                return links | vws::transform([](Link &link) -> DocumentLink {
                        return {
                                .range   = link.range,
                                .target  = std::move(link.target),
                                .tooltip = std::move(link.tooltip),
                        };
                }) | rng::to<std::vector<DocumentLink>>();
        }

        auto foldingRange(const FoldingRangeParams &params) -> Expected<std::variant<null, std::vector<FoldingRange>>>
                requires HasFoldingRange<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<Fold> folds = it->second.file->foldingRanges();
                if (folds.empty())
                        return nullptr;

                return folds | vws::transform([](Fold &fold) -> FoldingRange {
                        return {
                                .startLine      = fold.range.start.line,
                                .startCharacter = fold.range.start.character,
                                .endLine        = fold.range.end.line,
                                .endCharacter   = fold.range.end.character,
                                .kind           = fold.kind,
                                .collapsedText  = std::move(fold.preview)
                        };
                }) | rng::to<std::vector<FoldingRange>>();
        }

        auto selectionRange(const SelectionRangeParams &params) -> Expected<std::variant<null, std::vector<SelectionRange>>>
                requires HasSelectionRange<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<SelectionRange> selections;
                selections.reserve(params.positions.size());
                for (Position cursor : params.positions) {
                        std::vector<Range> ranges = it->second.file->selectionRanges(cursor);
                        if (ranges.empty())
                                continue;

                        SelectionRange root = {
                                .range = ranges.front()
                        };

                        SelectionRange *prev = &root;
                        for (size_t i = 1; i < ranges.size(); ++i) {
                                prev->parent = std::make_unique<SelectionRange>(ranges[i]);
                                prev         = prev->parent->get();
                        }

                        selections.push_back(std::move(root));
                }

                return selections;
        }

        auto documentSymbol(const DocumentSymbolParams &params) -> Expected<std::variant<null, std::vector<DocumentSymbol>, std::vector<SymbolInformation>>>
                requires HasSymbols<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<DocumentSymbol> symbols = it->second.file->symbols();
                if (symbols.empty())
                        return nullptr;

                return symbols;
        }

        auto workspaceSymbol(const WorkspaceSymbolParams &params) -> Expected<std::variant<null, std::vector<SymbolInformation>, std::vector<WorkspaceSymbol>>>
                requires HasSymbols<FileT>
        {
                namespace rng = std::ranges;
                namespace vws = std::views;

                if (not m_initialized)
                        return not_initialized_error;

                std::vector<WorkspaceSymbol> workspaceSymbols;
                for (const auto &[uri, file] : m_openDocuments) {
                        std::vector<DocumentSymbol> symbols = file.file->symbols();
                        if (not params.query.empty()) {
                                symbols = symbols
                                        | vws::filter([&query = params.query](const DocumentSymbol &symbol) -> bool {
                                                // TODO: fuzzy matching
                                                return symbol.name.contains(query);
                                        }) | rng::to<std::vector<DocumentSymbol>>();
                        }

                        if (symbols.empty())
                                continue;

                        workspaceSymbols.append_range(_convertDocumentSymbols(uri, symbols));
                }

                if (workspaceSymbols.empty())
                        return nullptr;

                return workspaceSymbols;
        }

        auto semanticTokensFull(const SemanticTokensParams &params) -> Expected<std::variant<null, SemanticTokens>>
                requires HasSemanticTokens<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                return SemanticTokens{
                        .data = _semanticTokens(it->second.file)
                };
        }

        auto semanticTokensRange(const SemanticTokensRangeParams &params) -> Expected<std::variant<null, SemanticTokens>>
                requires HasSemanticTokens<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<uinteger> tokens = _semanticTokens(it->second.file, params.range);
                if (tokens.empty())
                        return nullptr;

                return SemanticTokens{
                        .data = std::move(tokens)
                };
        }

        auto inlayHint(const InlayHintParams &params) -> Expected<std::variant<null, std::vector<InlayHint>>>
                requires HasInlayHints<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<InlayHint> hints= it->second.file->hints(params.range);
                if (hints.empty())
                        return nullptr;

                return hints;
        }

        auto inlayValue(const InlineValueParams &params) -> Expected<std::variant<null, std::vector<InlineValue>>>
                requires HasInlineValue<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<InlineValue> values = it->second.file->inlineValues(params.range, params.context);
                if (values.empty())
                        return nullptr;

                return values;
        }

        auto completion(const CompletionParams &params) -> Expected<std::variant<null, std::vector<CompletionItem>, CompletionList>>
                requires HasCompletion<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<CompletionItem> completions = it->second.file->completions(params.position);
                if (completions.empty())
                        return nullptr;

                return completions;
        }

        auto diagnostic(const DocumentDiagnosticParams &params) -> Expected<DocumentDiagnosticReport>
                requires HasDiagnostic<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                return RelatedFullDocumentDiagnosticReport{
                        FullDocumentDiagnosticReport{
                                .items = it->second.file->diagnostics()
                        }
                };
        }

        auto workspaceDiagnostic(const WorkspaceDiagnosticParams &) -> Expected<WorkspaceDiagnosticReport>
                requires HasDiagnostic<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                std::vector<WorkspaceDocumentDiagnosticReport> reports;
                reports.reserve(m_openDocuments.size());
                for (const auto &[uri, file] : m_openDocuments) {
                        WorkspaceFullDocumentDiagnosticReport report = {
                                FullDocumentDiagnosticReport{
                                        .items = file.file->diagnostics()
                                }
                        };
                        report.uri = uri;
                        reports.emplace_back(report);
                }

                return WorkspaceDiagnosticReport{
                        .items = reports
                };
        }

        auto signatureHelp(const SignatureHelpParams &params) -> Expected<std::variant<null, SignatureHelp>>
                requires HasSignatureHelp<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::optional<SignatureHelp> help = it->second.file->signatureHelp(params.position);
                if (not help.has_value() or help->signatures.empty())
                        return nullptr;

                return help.value();
        }

        auto codeAction(const CodeActionParams &params) -> Expected<std::variant<null, std::vector<std::variant<Command, CodeAction>>>>
                requires HasCodeAction<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<std::variant<Command, CodeAction>> actions = it->second.file->actions(params.range, params.context.only);
                if (actions.empty())
                        return nullptr;

                return actions;
        }

        auto documentColor(const DocumentColorParams &params) -> Expected<std::vector<ColorInformation>>
                requires HasColors<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                return it->second.file->colors();
        }

        auto colorPresentation(const ColorPresentationParams &params) -> Expected<std::vector<ColorPresentation>>
                requires HasColors<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                return it->second.file->placeColor(params.color, params.range);
        }

        auto format(const DocumentFormattingParams &params) -> Expected<std::variant<null, std::vector<TextEdit>>>
                requires HasFormatting<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<TextEdit> edits = it->second.file->format(params.options);
                if (edits.empty())
                        return nullptr;

                return edits;
        }

        auto formatRange(const DocumentRangeFormattingParams &params) -> Expected<std::variant<null, std::vector<TextEdit>>>
                requires HasFormatting<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<TextEdit> edits = it->second.file->format(params.options, params.range);
                if (edits.empty())
                        return nullptr;

                return edits;
        }

        auto formatRanges(const DocumentRangesFormattingParams &params) -> Expected<std::variant<null, std::vector<TextEdit>>>
                requires HasFormatting<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<TextEdit> edits;
                for (const Range &range : params.ranges)
                        edits.append_range(it->second.file->format(params.options, range));

                if (edits.empty())
                        return nullptr;

                return edits;
        }

        auto onTypeFormat(const DocumentOnTypeFormattingParams &params) -> Expected<std::variant<null, std::vector<TextEdit>>>
                requires HasFormatting<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<TextEdit> edits = it->second.file->format(params.options, params.position);
                if (edits.empty())
                        return nullptr;

                return edits;
        }

        auto rename(const RenameParams &params) -> Expected<std::variant<null, WorkspaceEdit>>
                requires HasRename<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                using Ctx = FileT::RefactorContextT;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::unordered_map<DocumentUri, std::vector<TextEdit>> edits;

                Ctx ctx                            = it->second.file->createRenameCtx(params.position, params.newName);
                edits[it->second.textDocument.uri] = it->second.file->rename(ctx);
                for (const auto &[uri, file] : m_openDocuments)
                        if (file.file != it->second.file)
                                edits[uri] = file.file->rename(ctx);

                if (edits.empty())
                        return nullptr;

                // TODO: support documentChanges (it allows for file rename and annotated text edits)
                return WorkspaceEdit{
                        .changes = std::move(edits)
                };
        }

        auto checkRename(const PrepareRenameParams &params) -> Expected<std::variant<null, Range, _PrepareRenameRequestResult1, _PrepareRenameRequestResult2>>
                requires HasPrepareRename<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                const std::expected<Range, string> rename = it->second.file->canRename(params.position);
                if (rename.has_value())
                        return rename.value();

                return std::unexpected(glz::rpc::error(
                        static_cast<glz::rpc::error_e>(ErrorCodes::RequestFailed),
                        std::nullopt,
                        rename.error())
                );
        }

        auto linkedEditing(const LinkedEditingRangeParams &params) -> Expected<std::variant<null, LinkedEditingRanges>>
                requires HasLinkedEditing<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                LinkedEditingRanges ranges = it->second.file->editingRanges(params.position);
                if (ranges.ranges.empty())
                        return nullptr;

                return ranges;
        }

        auto inlineCompletion(const InlineCompletionParams &params) -> Expected<std::variant<null, std::vector<InlineCompletionItem>, InlineCompletionList>>
                requires HasInlineCompletion<FileT>
        {
                if (not m_initialized)
                        return not_initialized_error;

                const auto it = m_openDocuments.find(params.textDocument.uri);
                if (it == m_openDocuments.end())
                        return internal_error;

                std::vector<InlineCompletionItem> completions = it->second.file->inlineCompletions(params.position);
                if (completions.empty())
                        return nullptr;

                return completions;
        }

        auto _semanticTokens(const std::unique_ptr<FileT> &file, const std::optional<Range> &range = std::nullopt) const -> std::vector<uinteger>
                requires HasSemanticTokens<FileT>
        {
                std::vector<uinteger> sem;
                sem.reserve(250);

                TokenItT it        = file->begin();
                const TokenItT end = file->end();
                if (range.has_value())
                        while (it != end and *it < range->start)
                                ++it;

                uinteger prevLine      = 0;
                uinteger prevCharacter = 0;
                for (; it != end; ++it) {
                        const TokenT &token = *it;
                        const auto type     = static_cast<uinteger>(token.type());
                        const auto mod      = static_cast<uinteger>(token.modifier());
                        if (type == static_cast<uinteger>(TokenT::invalid_type))
                                continue;

                        if (range.has_value() and token > range->end)
                                break;

                        const uinteger deltaLine = token.line - prevLine;
                        sem.push_back(deltaLine);
                        sem.push_back(deltaLine ? token.start : token.start - prevCharacter);
                        sem.push_back(token.length);
                        sem.push_back(static_cast<uinteger>(type));
                        sem.push_back(static_cast<uinteger>(mod));

                        prevLine      = token.line;
                        prevCharacter = token.start;
                }

                return sem;
        }

        static auto _getCapabilities() -> ServerCapabilities
        {
                namespace rng = std::ranges;

                ServerCapabilities capabilities = {
                        .positionEncoding                = PositionEncodingKind::UTF16,
                        .textDocumentSync                = TextDocumentSyncOptions{
                                .openClose = true,
                                .change    = TextDocumentSyncKind::Incremental
                        },
                        .completionProvider              = HasCompletion<FileT> ? std::optional(CompletionOptions{
                                .resolveProvider = false,
                                .completionItem  = CompletionOptions::_CompletionItem{
                                        .labelDetailsSupport = true
                                }
                        }) : std::nullopt,
                        .hoverProvider                   = HasHover<FileT> ? std::optional(true) : std::nullopt,
                        .signatureHelpProvider           = HasSignatureHelp<FileT> ? std::optional(SignatureHelpOptions{
                                .triggerCharacters = std::vector<string>{ "(" }
                        }) : std::nullopt,
                        .declarationProvider             = HasResolve<FileT> ? std::optional(true) : std::nullopt,
                        .definitionProvider              = HasResolve<FileT> ? std::optional(true) : std::nullopt,
                        .typeDefinitionProvider          = HasResolve<FileT> ? std::optional(true) : std::nullopt,
                        .implementationProvider          = HasResolve<FileT> ? std::optional(true) : std::nullopt,
                        .referencesProvider              = HasReferences<FileT> ? std::optional(true) : std::nullopt,
                        .documentHighlightProvider       = HasHighlight<FileT> ? std::optional(true) : std::nullopt,
                        .documentSymbolProvider          = HasSymbols<FileT> ? std::optional(true) : std::nullopt,
                        .codeActionProvider              = HasCodeAction<FileT> ? std::optional(CodeActionOptions{
                                // TODO action kinds
                                .resolveProvider = false
                        }) : std::nullopt,
                        .codeLensProvider                = HasCodeLens<FileT> ? std::optional(CodeLensOptions{
                                .resolveProvider = false
                        }) : std::nullopt,
                        .documentLinkProvider            = HasLinks<FileT> ? std::optional(DocumentLinkOptions{
                                .resolveProvider = false
                        }) : std::nullopt,
                        .colorProvider                   = HasColors<FileT> ? std::optional(true) : std::nullopt,
                        .documentFormattingProvider      = HasFormatting<FileT> ? std::optional(true) : std::nullopt,
                        .documentRangeFormattingProvider = HasFormatting<FileT> ? std::optional(DocumentRangeFormattingOptions{
                                .rangesSupport = true
                        }) : std::nullopt,
                        .renameProvider                  = HasRename<FileT> ? std::optional(RenameOptions{
                                .prepareProvider         = HasPrepareRename<FileT> ? std::optional(true) : std::nullopt
                        }) : std::nullopt,
                        .foldingRangeProvider            = HasFoldingRange<FileT> ? std::optional(true) : std::nullopt,
                        // TODO: .executeCommandProvider           = false,
                        .selectionRangeProvider          = HasSelectionRange<FileT> ? std::optional(true) : std::nullopt,
                        .linkedEditingRangeProvider      = HasLinkedEditing<FileT> ? std::optional(true) : std::nullopt,
                        .callHierarchyProvider           = HasHierarchy<FileT> ? std::optional(true) : std::nullopt,
                        // TODO: .monikerProvider                  = HasMoniker<FileT>,
                        .typeHierarchyProvider           = HasHierarchy<FileT> ? std::optional(true) : std::nullopt,
                        .inlineValueProvider             = HasInlineValue<FileT> ? std::optional(true) : std::nullopt,
                        .inlayHintProvider               = HasInlayHints<FileT> ? std::optional(InlayHintOptions{
                                .resolveProvider = false
                        }) : std::nullopt,
                        .workspaceSymbolProvider         = HasSymbols<FileT> ? std::optional(true) : std::nullopt,
                        .inlineCompletionProvider        = HasInlineCompletion<FileT> ? std::optional(true) : std::nullopt,
                        .textDocument                    = ServerCapabilities::_TextDocument{},
                        // TODO: .workspace                       = std::nullopt,
                };

                if constexpr (Notebook<FileT>)
                        capabilities.notebookDocumentSync = NotebookDocumentSyncOptions{
                                .notebookSelector = FileT::notebook_selector,
                                .save             = true
                        };

                if constexpr (HasSemanticTokens<FileT>)
                        capabilities.semanticTokensProvider = SemanticTokensOptions{
                                .legend = {
                                        .tokenTypes     = FileT::token_types,
                                        .tokenModifiers = FileT::token_modifiers
                                },
                                .range  = true,
                                .full   = SemanticTokensOptions::_Full{
                                        .delta = false
                                }
                        };

                if constexpr (HasOnTypeFormatting<FileT>)
                        capabilities.documentOnTypeFormattingProvider = DocumentOnTypeFormattingOptions{
                                .firstTriggerCharacter = FileT::on_type_trigger_chars[0],
                                .moreTriggerCharacters = FileT::on_type_trigger_chars.size() > 1 ?
                                        std::optional(std::vector(&FileT::on_type_trigger_chars[1], FileT::on_type_trigger_chars.end())) :
                                        std::nullopt
                        };

                if constexpr (HasDiagnostic<FileT>) {
                        capabilities.diagnosticProvider = DiagnosticOptions{
                                .internalDependencies = FileT::diagnostic_internal_dependencies,
                                // TODO: .workspaceDiagnostics = true
                        };

                        capabilities.textDocument->diagnostic = ServerCapabilities::_TextDocument::_Diagnostic{
                                .markupMessageSupport = true
                        };
                }

                return capabilities;
        }

}; // class LSP

} // namespace LSP