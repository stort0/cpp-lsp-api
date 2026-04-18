# C++ LSP API

Wrapper of the `glaze` json library for use with `lsp` server and client.
The library uses `C++23`.

## Language Server

The `LSP::LanguageServer` class takes a template type which uses to parse and
handle the client requests. The server features are checked through **concepts**.

> The `FileT` template is used as a const variable and is **never** mutated. The
> server will **replace** the old one when the file is updated.
> All functions used by the `LanguageServer` **must** be `const`.

> If a `TokenT` type token spans across multiple lines, the method `split()` is
> required for semantic tokens (otherwise token will be skipped).

### Disable catch of exception

To forward all exception that happen during the language server execution,
define `CPP_LSP_API_DISABLE_CATCH` or `CPP_LSP_API_DISABLE_CATCH_DEBUG` to only
disable it during debug mode.

### Complete implementation of a file parser

```c++
class MyTokenType {
public:
        /*
         * Required elements
         */
        auto range() const -> Range;

        /*
         * Required for semantic tokens
         */
        static const uinteger invalid_type;
        auto operator<=> (Position pos) const -> std::weak_ordering;  // == means the position is inside range()
        auto type() const -> uinteger;
        auto modifier() const -> uinteger;
        auto split() const -> std::vector<MyTokenType>;  // optional
    
};

class MyRefactorContext {

};

class MyLanguageFile {
public:
        /*
         * Required elements
         */
        using TokenT    = MyTokenType;  // MUST BE A BASIC TYPE (NO CVREF)
        using TokenPtrT = const MyTokenType *;

        static auto parse(const std::string &text) -> std::unique_ptr<MyLanguageFile>;

        /*
         * Required for notebook parsing
         */
        static const NotebookDocumentSyncOptions::_NotebookSelector notebook_selector;
        static auto parse(const std::string &text, NotebookCellKind kind, const std::vector<MyLanguageFile *> &previousCells) -> std::unique_ptr<MyLanguageFile>;

        /*
         * Required for edits on file save
         */
        auto save() const -> std::vector<TextEdit>;

        /*
         * Required for call/type/implementation resolution
         */
        auto resolve(Position pos) const -> TokenPtrT;

        /*
         * Required for element references
         */
        auto references(Position pos) const -> std::vector<TokenPtrT>;

        /*
         * Required for semantic tokens
         */
        static const std::vector<string> token_types;
        static const std::vector<string> token_modifiers;
        auto visibleTokens() const -> std::ranges::input_range;  // Any range with value_type == Token
        // optional, should be set to true if visibleTokens() returns a sorted array
        // based on the token Range.    
        static const bool ordered_visible_tokens;
    
        /*
         * Required for hover
         */
        auto hover(Position pos) const -> std::optional<MarkupContent>;

        /*
         * Required for code lenses
         */
        auto codeLenses() const -> std::vector<CodeLens>;

        /*
         * Required for code diagnostics
         */
        static const bool diagnostic_internal_dependencies;
        auto diagnostics() const -> std::vector<Diagnostic>;

        /*
         * Required for call/type hierarchy
         */
        auto roots(Position pos) const -> std::vector<LSP::HierarchyItem>;
        auto children(const std::optional<LSPAny> &data) const -> std::vector<LSP::HierarchyItem>;
        auto parents(const std::optional<LSPAny> &data) const -> std::vector<LSP::HierarchyItem>;

        /*
         * Required for highlights
         */
        auto highlights(Position pos) const -> std::vector<DocumentHighlight>;

        /*
         * Required for links
         */
        auto links() const -> std::vector<LSP::Link>;

        /*
         * Required for folding ranges
         */
        auto foldingRanges() const -> std::vector<LSP::Fold>;

        /*
         * Required for folding ranges
         */
        auto selectionRanges(Position pos) const -> std::vector<Range>;  // RANGES MUST BE INCREASINGLY BIGGER,
                                                                         // ELEMENTS MUST CONTAIN THE PREVIOUS ONES

        /*
         * Required for symbols
         */
        auto symbols() const -> std::vector<DocumentSymbol>;

        /*
         * Required for inlay hints
         */
        auto hints(const Range &range) const -> std::vector<InlayHint>;

        /*
         * Required for inlay hints
         */
        auto inlineValues(const Range &range, const InlineValueContext &ctx) const -> std::vector<InlineValue>;

        /*
         * Required for completions
         */
        auto completions(Position pos) const -> std::vector<CompletionItem>;

        /*
         * Required for signature help
         */
        auto signatureHelp(Position pos) const -> std::optional<SignatureHelp>;

        /*
         * Required for code actions
         */
        auto actions(const Range &range, std::optional<CodeActionKind> kind) const -> std::vector<std::variant<Command, CodeAction>>;

        /*
         * Required for colors
         */
        auto colors() const -> std::vector<ColorInformation>;
        auto placeColor(const Color &color, const Range &range) const -> std::vector<ColorPresentation>;

        /*
         * Required for format
         */
        auto format(const FormattingOptions &opts, const std::optional<Range> &range) const -> std::vector<TextEdit>;

        /*
         * Required for on type formatting
         */
        static const std::vector<string> on_type_trigger_chars;
        auto format(const FormattingOptions &opts, Position pos) const -> std::vector<TextEdit>;

        /*
         * Required for element rename
         */
        using RefactorContextT = MyRefactorContext;  // MUST BE A BASIC TYPE (NO CVREF)
        auto createRenameCtx(Position pos, const string &newName) const -> RefactorContextT;
        auto rename(const RefactorContextT &ctx) const -> std::vector<TextEdit>;
        
        /*
         * Required rename check
         */
        auto canRename(Position pos) const -> std::expected<Range, string>;
        
        /*
         * Required linked editing ranges
         */
        auto editingRanges(Position pos) const -> LinkedEditingRanges;
        
        /*
         * Required inline completions
         */
        auto inlineCompletions(Position pos) const -> std::vector<InlineCompletionItem>;
        
};
```

## Language client

> TODO
