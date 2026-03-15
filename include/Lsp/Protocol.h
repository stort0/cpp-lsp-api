//
// Created by storto on 06/08/2025.
//

#pragma once

#include <glaze/ext/jsonrpc.hpp>
#include <unordered_map>
#include <string_view>
#include <optional>
#include <utility>
#include <variant>
#include <cstdint>
#include <vector>
#include <string>
#include <array>

#include <Lsp/_StringEnum.h>

struct _Void {};

static constexpr std::array<std::string_view, 3> EOL = { "\r\n", "\n", "\r" };

/**
 * Defines an integer number in the range of -2^31 to 2^31 - 1.
 */
using integer  = int32_t;

/**
 * Defines an unsigned integer number in the range of 0 to 2^31 - 1.
 */
using uinteger = uint32_t;

/**
 * Defines a decimal number. Since decimal numbers are very
 * rare in the language server specification we denote the
 * exact range with every decimal using the mathematics
 * interval notation (e.g. [0, 1] denotes all decimals d with
 * 0 <= d <= 1.
 */
using decimal  = float;

using boolean  = bool;
using string   = std::string;
using null     = std::nullptr_t;

/**
 * The LSP any type
 *
 * @version 3.17.0
 */
using LSPAny = glz::generic;

/**
 * LSP object definition.
 *
 * @version 3.17.0
 */
using LSPObject = std::unordered_map<std::string, LSPAny>;

/**
 * LSP arrays.
 *
 * @version 3.17.0
 */
using LSPArray = std::vector<LSPAny>;

/**
 * The kind of resource operations supported by the client.
 */
STRING_ENUM(ResourceOperationKind,
        Create, "create",
        Rename, "rename",
        Delete, "delete"
)

STRING_ENUM(FailureHandlingKind,
        Abort, "abort",
        Transactional, "transactional",
        TextOnlyTransactional, "textOnlyTransactional",
        Undo, "undo"
)

struct WorkspaceEditClientCapabilities {
        struct _ChangeAnnotationSupport {
                /**
                 * Whether the client groups edits with equal labels into tree nodes,
                 * for instance all edits labelled with "Changes in Strings" would
                 * be a tree node.
                 */
                std::optional<boolean> groupsOnLabel;
        };

        /**
         * The client supports versioned document changes in @code WorkspaceEdit@endcode s
         */
        std::optional<boolean> documentChanges;

        /**
         * The resource operations the client supports. Clients should at least
         * support 'create', 'rename' and 'delete' files and folders.
         *
         * @version 3.13.0
         */
        std::optional<std::vector<ResourceOperationKind>> resourceOperations;

        /**
         * The failure handling strategy of a client if applying the workspace edit
         * fails.
         *
         * @version 3.13.0
         */
        std::optional<FailureHandlingKind> failureHandling;

        /**
         * Whether the client normalizes line endings to the client specific
         * setting.
         * If set to @code true@endcode the client will normalize line ending characters
         * in a workspace edit to the client specific new line character(s).
         *
         * @version 3.16.0
         */
        std::optional<boolean> normalizesLineEndings;

        /**
         * Whether the client in general supports change annotations on text edits,
         * create file, rename file and delete file changes.
         *
         * @version 3.16.0
         */
        std::optional<_ChangeAnnotationSupport> changeAnnotationSupport;

        /**
         * Whether the client supports @code WorkspaceEditMetadata@endcode in
         * @code WorkspaceEdit@endcode s.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<boolean> metadataSupport;

        /**
         * Whether the client supports snippets as text edits.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<boolean> snippetEditSupport;
};

struct DidChangeConfigurationClientCapabilities {
        /**
         * Did change configuration notification supports dynamic registration.
         *
         * @version 3.6.0 to support the new pull model.
         */
        std::optional<boolean> dynamicRegistration;
};

struct DidChangeWatchedFilesClientCapabilities {
        /**
         * Did change watched files notification supports dynamic registration.
         * Please note that the current protocol doesn't support static
         * configuration for file changes from the server side.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Whether the client has support for relative patterns
         * or not.
         *
         * @version 3.17.0
         */
        std::optional<boolean> relativePatternSupport;
};

/**
 * A symbol kind.
 */
enum class SymbolKind : uinteger {
        File          = 1,
        Module        = 2,
        Namespace     = 3,
        Package       = 4,
        Class         = 5,
        Method        = 6,
        Property      = 7,
        Field         = 8,
        Constructor   = 9,
        Enum          = 10,
        Interface     = 11,
        Function      = 12,
        Variable      = 13,
        Constant      = 14,
        String        = 15,
        Number        = 16,
        Boolean       = 17,
        Array         = 18,
        Object        = 19,
        Key           = 20,
        Null          = 21,
        EnumMember    = 22,
        Struct        = 23,
        Event         = 24,
        Operator      = 25,
        TypeParameter = 26

};

/**
 * Symbol tags are extra annotations that tweak the rendering of a symbol.
 *
 * @version 3.16
 */
enum class SymbolTag : uinteger {
        /**
         * Render a symbol as obsolete, usually using a strike-out.
         */
        Deprecated = 1

};

struct WorkspaceSymbolClientCapabilities {
        struct _SymbolKind {
                /**
                 * The symbol kind values the client supports. When this
                 * property exists the client also guarantees that it will
                 * handle values outside its set gracefully and falls back
                 * to a default value when unknown.
                 *
                 * If this property is not present the client only supports
                 * the symbol kinds from @code File@endcode to @code Array@endcode
                 * as defined in the initial version of the protocol.
                 */
                std::optional<std::vector<SymbolKind>> valueSet;
        };

        struct _TagSupport {
                /**
                 * The tags supported by the client.
                 */
                std::optional<std::vector<SymbolTag>> valueSet;
        };

        struct _ResolveSupport {
                /**
                 * The properties that a client can resolve lazily. Usually
                 * @code location.range@endcode
                 */
                std::vector<string> properties;
        };

        /**
         * Symbol request supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Specific capabilities for the @code SymbolKind@endcode in the
         * @code workspace/symbol@endcode request.
         */
        std::optional<_SymbolKind> symbolKind;

        /**
         * The client supports tags on @code SymbolInformation@endcode and
         * @code WorkspaceSymbol@endcode. Clients supporting tags have to handle
         * unknown tags gracefully.
         *
         * @version 3.16.0
         */
        std::optional<_TagSupport> tagSupport;

        /**
         * The client support partial workspace symbols. The client will send the
         * request @code workspaceSymbol/resolve@endcode to the server to resolve
         * additional properties.
         *
         * @version 3.17.0 - proposedState
         */
        std::optional<_ResolveSupport> resolveSupport;
};

struct ExecuteCommandClientCapabilities {
        /**
         * Execute command supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;
};

struct SemanticTokensWorkspaceClientCapabilities {
        /**
         * Whether the client implementation supports a refresh request sent from
         * the server to the client.
         *
         * Note that this event is global and will force the client to refresh all
         * semantic tokens currently shown. It should be used with absolute care
         * and is useful for situation where a server for example detect a project
         * wide change that requires such a calculation.
         */
        std::optional<boolean> refreshSupport;
};

struct CodeLensWorkspaceClientCapabilities {
        /**
         * Whether the client implementation supports a refresh request sent from the
         * server to the client.
         *
         * Note that this event is global and will force the client to refresh all
         * code lenses currently shown. It should be used with absolute care and is
         * useful for situation where a server for example detect a project wide
         * change that requires such a calculation.
         */
        std::optional<boolean> refreshSupport;
};

/**
 * Client workspace capabilities specific to inline values.
 *
 * @version 3.17.0
 */
struct InlineValueWorkspaceClientCapabilities {
        /**
         * Whether the client implementation supports a refresh request sent from
         * the server to the client.
         *
         * Note that this event is global and will force the client to refresh all
         * inline values currently shown. It should be used with absolute care and
         * is useful for situation where a server for example detect a project wide
         * change that requires such a calculation.
         */
        std::optional<boolean> refreshSupport;
};

/**
 * Client workspace capabilities specific to inlay hints.
 *
 * @version 3.17.0
 */
struct InlayHintWorkspaceClientCapabilities {
        /**
         * Whether the client implementation supports a refresh request sent from
         * the server to the client.
         *
         * Note that this event is global and will force the client to refresh all
         * inlay hints currently shown. It should be used with absolute care and
         * is useful for situation where a server for example detects a project wide
         * change that requires such a calculation.
         */
        std::optional<boolean> refreshSupport;
};

/**
 * Workspace client capabilities specific to diagnostic pull requests.
 *
 * @version 3.17.0
 */
struct DiagnosticWorkspaceClientCapabilities {
        /**
         * Whether the client implementation supports a refresh request sent from
         * the server to the client.
         *
         * Note that this event is global and will force the client to refresh all
         * pulled diagnostics currently shown. It should be used with absolute care
         * and is useful for situation where a server for example detects a project
         * wide change that requires such a calculation.
         */
        std::optional<boolean> refreshSupport;
};

/**
 * Client capabilities for a text document content provider.
 *
 * @version 3.18.0
 */
struct TextDocumentContentClientCapabilities {
        /**
         * Text document content provider supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;
};

struct FoldingRangeWorkspaceClientCapabilities {
        /**
         * Whether the client implementation supports a refresh request sent from the
         * server to the client.
         *
         * Note that this event is global and will force the client to refresh all
         * folding ranges currently shown. It should be used with absolute care and is
         * useful for situation where a server, for example, detects a project wide
         * change that requires such a calculation.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<boolean> refreshSupport;
};

struct TextDocumentSyncClientCapabilities {
        /**
         * Whether text document synchronization supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports sending will save notifications.
         */
        std::optional<boolean> willSave;

        /**
         * The client supports sending a will save request and
         * waits for a response providing text edits which will
         * be applied to the document before it is saved.
         */
        std::optional<boolean> willSaveWaitUntil;

        /**
         * The client supports did save notifications.
         */
        std::optional<boolean> didSave;
};

/**
 * Completion item tags are extra annotations that tweak the rendering of a
 * completion item.
 *
 * @version 3.15.0
 */
enum class CompletionItemTag {
        /**
         * Render a completion as obsolete, usually using a strike-out.
         */
        Deprecated = 1

};

/**
 * How whitespace and indentation is handled during completion
 * item insertion.
 *
 * @version 3.16.0
 */
enum class InsertTextMode {
        /**
         * The insertion or replace strings is taken as it is. If the
         * value is multi line the lines below the cursor will be
         * inserted using the indentation defined in the string value.
         * The client will not apply any kind of adjustments to the
         * string.
         */
        asIs = 1,

        /**
         * The editor adjusts leading whitespace of new lines so that
         * they match the indentation up to the cursor of the line for
         * which the item is accepted.
         *
         * Consider a line like this: <2tabs><cursor><3tabs>foo. Accepting a
         * multi line completion item is indented using 2 tabs and all
         * following lines inserted will be indented using 2 tabs as well.
         */
        adjustIndentation = 2

};

/**
 * Describes the content type that a client supports in various
 * result literals like @code Hover@endcode, @code ParameterInfo@endcode or
 * @code CompletionItem@endcode.
 *
 * Please note that @code MarkupKinds@endcode must not start with a @code $@endcode. This kinds
 * are reserved for internal usage.
 */
STRING_ENUM(MarkupKind,
        PlainText, "plaintext",
        MarkDown, "markdown"
)

/**
 * The kind of a completion entry.
 */
enum class CompletionItemKind {
        Text          = 1,
        Method        = 2,
        Function      = 3,
        Constructor   = 4,
        Field         = 5,
        Variable      = 6,
        Class         = 7,
        Interface     = 8,
        Module        = 9,
        Property      = 10,
        Unit          = 11,
        Value         = 12,
        Enum          = 13,
        Keyword       = 14,
        Snippet       = 15,
        Color         = 16,
        File          = 17,
        Reference     = 18,
        Folder        = 19,
        EnumMember    = 20,
        Constant      = 21,
        Struct        = 22,
        Event         = 23,
        Operator      = 24,
        TypeParameter = 25

};

struct CompletionClientCapabilities {
        struct _CompletionItem {
                struct _CompletionTagSupport {
                        /**
                         * The tags supported by the client.
                         */
                        std::vector<CompletionItemTag> valueSet;
                };

                struct _ResolveSupport {
                        /**
                         * The properties that a client can resolve lazily.
                         */
                        std::vector<string> properties;
                };

                struct _InsertTextModeSupport {
                        std::vector<InsertTextMode> valueSet;
                };

                /**
                 * Client supports snippets as insert text.
                 *
                 * A snippet can define tab stops and placeholders with @code $1@endcode, @code $2@endcode
                 * and @code ${3:foo}@endcode. @code $0@endcode defines the final tab stop, it defaults to
                 * the end of the snippet. Placeholders with equal identifiers are
                 * linked, that is typing in one will update others too.
                 */
                std::optional<boolean> snippetSupport;

                /**
                 * Client supports commit characters on a completion item.
                 */
                std::optional<boolean> commitCharactersSupport;

                /**
                 * Client supports the follow content formats for the documentation
                 * property. The order describes the preferred format of the client.
                 */
                std::optional<std::vector<MarkupKind>> documentationFormat;

                /**
                 * Client supports the deprecated property on a completion item.
                 */
                std::optional<boolean> deprecatedSupport;

                /**
                 * Client supports the preselect property on a completion item.
                 */
                std::optional<boolean> preselectSupport;

                /**
                 * Client supports the tag property on a completion item. Clients
                 * supporting tags have to handle unknown tags gracefully. Clients
                 * especially need to preserve unknown tags when sending a completion
                 * item back to the server in a resolve call.
                 *
                 * @version 3.15.0
                 */
                std::optional<_CompletionTagSupport> tagSupport;

                /**
                 * Client supports insert replace edit to control different behavior if
                 * a completion item is inserted in the text or should replace text.
                 *
                 * @version 3.16.0
                 */
                std::optional<boolean> insertReplaceSupport;

                /**
                 * Indicates which properties a client can resolve lazily on a
                 * completion item. Before version 3.16.0 only the predefined properties
                 * @code documentation@endcode and @code detail@endcode could be resolved lazily.
                 *
                 * @version 3.16.0
                 */
                std::optional<_ResolveSupport> resolveSupport;

                /**
                 * The client supports the @code insertTextMode@endcode property on
                 * a completion item to override the whitespace handling mode
                 * as defined by the client (see @code insertTextMode@endcode).
                 *
                 * @version 3.16.0
                 */
                std::optional<_InsertTextModeSupport> insertTextModeSupport;

                /**
                 * The client has support for completion item label
                 * details (see also @code CompletionItemLabelDetails@endcode).
                 *
                 * @version 3.17.0
                 */
                std::optional<boolean> labelDetailsSupport;
        };

        struct _CompletionItemKind {
                /**
                 * The completion item kind values the client supports. When this
                 * property exists the client also guarantees that it will
                 * handle values outside its set gracefully and falls back
                 * to a default value when unknown.
                 *
                 * If this property is not present the client only supports
                 * the completion items kinds from @code Text@endcode to @code Reference@endcode as defined in
                 * the initial version of the protocol.
                 */
                std::optional<std::vector<CompletionItemKind>> valueSet;
        };

        struct _CompletionList {
                /**
                 * The client supports the following itemDefaults on
                 * a completion list.
                 *
                 * The value lists the supported property names of the
                 * @code CompletionList.itemDefaults@endcode object. If omitted
                 * no properties are supported.
                 *
                 * @version 3.17.0
                 */
                std::optional<std::vector<string>> itemDefaults;

                /**
                 * Specifies whether the client supports @code CompletionList.applyKind@endcode to
                 * indicate how supported values from @code completionList.itemDefaults@endcode
                 * and @code completion@endcode will be combined.
                 *
                 * If a client supports @code applyKind@endcode it must support it for all fields
                 * that it supports that are listed in @code CompletionList.applyKind@endcode. This
                 * means when clients add support for new/future fields in completion
                 * items the MUST also support merge for them if those fields are
                 * defined in @code CompletionList.applyKind@endcode.
                 *
                 * @version 3.18.0
                 */
                std::optional<boolean> applyKindSupport;
        };

        /**
         * Whether completion supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports the following @code CompletionItem@endcode specific
         * capabilities.
         */
        std::optional<_CompletionItem> completionItem;

        std::optional<_CompletionItemKind> completionItemKind;

        /**
         * The client supports to send additional context information for a
         * @code textDocument/completion@endcode request.
         */
        std::optional<boolean> contextSupport;

        /**
         * The client's default when the completion item doesn't provide a
         * @code insertTextMode@endcode property.
         *
         * @version 3.17.0
         */
        std::optional<InsertTextMode> insertTextMode;

        /**
         * The client supports the following @code CompletionList@endcode specific
         * capabilities.
         *
         * @version 3.17.0
         */
        std::optional<_CompletionList> completionList;
};

struct HoverClientCapabilities {
        /**
         * Whether hover supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Client supports the follow content formats if the content
         * property refers to a @code literal of type MarkupContent@endcode.
         * The order describes the preferred format of the client.
         */
        std::optional<std::vector<MarkupKind>> contentFormat;
};

struct SignatureHelpClientCapabilities {
        struct _SignatureInformation {
                struct _ParameterInformation {
                        /**
                         * The client supports processing label offsets instead of a
                         * simple label string.
                         *
                         * @version 3.14.0
                         */
                        std::optional<boolean> labelOffsetSupport;
                };

                /**
                 * Client supports the follow content formats for the documentation
                 * property. The order describes the preferred format of the client.
                 */
                std::optional<std::vector<MarkupKind>> documentationFormat;

                /**
                 * Client capabilities specific to parameter information.
                 */
                std::optional<_ParameterInformation> parameterInformation;

                /**
                 * The client supports the @code activeParameter@endcode property on
                 * @code SignatureInformation@endcode literal.
                 *
                 * @version 3.16.0
                 */
                std::optional<boolean> activeParameterSupport;

                /**
                 * The client supports the @code activeParameter@endcode property on
                 * @code SignatureHelp@endcode/@code SignatureInformation@endcode
                 * being set to @code null@endcode to indicate that no parameter should be active.
                 *
                 * @version 3.18.0
                 */
                std::optional<boolean> noActiveParameterSupport;
        };

        /**
         * Whether signature help supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports the following @code SignatureInformation@endcode
         * specific properties.
         */
        std::optional<_SignatureInformation> signatureInformation;

        /**
         * The client supports to send additional context information for a
         * @code textDocument/signatureHelp@endcode request. A client that opts into
         * contextSupport will also support the @code retriggerCharacters@endcode on
         * @code SignatureHelpOptions@endcode.
         *
         * @version 3.15.0
         */
        std::optional<boolean> contextSupport;
};

struct DeclarationClientCapabilities {
        /**
         * Whether declaration supports dynamic registration. If this is set to
         * @code true@endcode the client supports the new @code DeclarationRegistrationOptions@endcode
         * return value for the corresponding server capability as well.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports additional metadata in the form of declaration links.
         */
        std::optional<boolean> linkSupport;
};

struct DefinitionClientCapabilities {
        /**
         * Whether definition supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports additional metadata in the form of definition links.
         *
         * @version 3.14.0
         */
        std::optional<boolean> linkSupport;
};

struct TypeDefinitionClientCapabilities {
        /**
         * Whether implementation supports dynamic registration. If this is set to
         * @code true@endcode the client supports the new @code TypeDefinitionRegistrationOptions@endcode
         * return value for the corresponding server capability as well.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports additional metadata in the form of definition links.
         *
         * @version 3.14.0
         */
        std::optional<boolean> linkSupport;
};

struct ImplementationClientCapabilities {
        /**
         * Whether implementation supports dynamic registration. If this is set to
         * @code true@endcode the client supports the new @code ImplementationRegistrationOptions@endcode
         * return value for the corresponding server capability as well.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports additional metadata in the form of definition links.
         *
         * @version 3.14.0
         */
        std::optional<boolean> linkSupport;
};

struct ReferenceClientCapabilities {
        /**
         * Whether references supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;
};

struct DocumentHighlightClientCapabilities {
        /**
         * Whether document highlight supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;
};

struct DocumentSymbolClientCapabilities {
        struct _SymbolKind {
                /**
                 * The symbol kind values the client supports. When this
                 * property exists the client also guarantees that it will
                 * handle values outside its set gracefully and falls back
                 * to a default value when unknown.
                 *
                 * If this property is not present the client only supports
                 * the symbol kinds from @code File@endcode to @code Array@endcode as defined in
                 * the initial version of the protocol.
                 */
                std::optional<std::vector<SymbolKind>> valueSet;
        };

        struct _TagSupport {
                /**
                 * The tags supported by the client.
                 */
                std::vector<SymbolTag> valueSet;
        };

        /**
         * Whether document symbol supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Specific capabilities for the @code SymbolKind@endcode in the
         * @code textDocument/documentSymbol@endcode request.
         */
        std::optional<_SymbolKind> symbolKind;

        /**
         * The client supports hierarchical document symbols.
         */
        std::optional<boolean> hierarchicalDocumentSymbolSupport;

        /**
         * The client supports tags on @code SymbolInformation@endcode. Tags are supported on
         * @code DocumentSymbol@endcode if @code hierarchicalDocumentSymbolSupport@endcode is set to true.
         * Clients supporting tags have to handle unknown tags gracefully.
         *
         * @version 3.16.0
         */
        std::optional<_TagSupport> tagSupport;

        /**
         * The client supports an additional label presented in the UI when
         * registering a document symbol provider.
         *
         * @version 3.16.0
         */
        std::optional<boolean> labelSupport;
};

/**
 * The kind of a code action.
 *
 * Kinds are a hierarchical list of identifiers separated by @code .@endcode,
 * e.g. @code "refactor.extract.function"@endcode.
 *
 * The set of kinds is open and client needs to announce the kinds it supports
 * to the server during initialization.
 */
STRING_ENUM(CodeActionKind,
        Empty, "",
        QuickFix, "quickfix",
        Refactor, "refactor",
        RefactorExtract, "refactor.extract",
        RefactorInline, "refactor.inline",
        RefactorMove, "refactor.move",
        RefactorRewrite, "refactor.rewrite",
        Source, "source",
        SourceOrganizeImports, "source.organizeImports",
        SourceFixAll, "source.fixAll",
        Notebook, "notebook"
)

/**
 * Code action tags are extra annotations that tweak the behavior of a code action.
 *
 * @version 3.18.0 - proposed
 */
enum class CodeActionTag {
        /**
         * Marks the code action as LLM-generated.
         */
        LLMGenerated

};

struct CodeActionClientCapabilities {
        struct _CodeActionLiteralSupport {
                /**
                 * The code action kind is supported with the following value
                 * set.
                 */
                struct {
                        /**
                         * The code action kind values the client supports. When this
                         * property exists the client also guarantees that it will
                         * handle values outside its set gracefully and falls back
                         * to a default value when unknown.
                         */
                        std::vector<CodeActionKind> valueSet;
                } codeActionKind;
        };

        struct _ResolveSupport {
                /**
                 * The properties that a client can resolve lazily.
                 */
                std::vector<string> properties;
        };

        struct _TagSupport {
                /**
                 * The tags supported by the client.
                 */
                std::vector<CodeActionTag> valueSet;
        };

        /**
         * Whether code action supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports code action literals as a valid
         * response of the @code textDocument/codeAction@endcode request.
         *
         * @version 3.8.0
         */
        std::optional<_CodeActionLiteralSupport> codeActionLiteralSupport;

        /**
         * Whether code action supports the @code isPreferred@endcode property.
         *
         * @version 3.15.0
         */
        std::optional<boolean> isPreferredSupport;

        /**
         * Whether code action supports the @code disabled@endcode property.
         *
         * @version 3.16.0
         */
        std::optional<boolean> disabledSupport;

        /**
         * Whether code action supports the @code data@endcode property which is
         * preserved between a @code textDocument/codeAction@endcode and a
         * @code codeAction/resolve@endcode request.
         *
         * @version 3.16.0
         */
        std::optional<boolean> dataSupport;

        /**
         * Whether the client supports resolving additional code action
         * properties via a separate @code codeAction/resolve@endcode request.
         *
         * @version 3.16.0
         */
        std::optional<_ResolveSupport> resolveSupport;

        /**
         * Whether the client honors the change annotations in
         * text edits and resource operations returned via the
         * @code CodeAction#edit@endcode property by for example presenting
         * the workspace edit in the user interface and asking
         * for confirmation.
         *
         * @version 3.16.0
         */
        std::optional<boolean> honorsChangeAnnotations;

        /**
         * Whether the client supports documentation for a class of code actions.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<boolean> documentationSupport;

        /**
         * Client supports the tag property on a code action. Clients
         * supporting tags have to handle unknown tags gracefully.
         *
         * @version 3.18.0 - proposed
         */
        std::optional<_TagSupport> tagSupport;
};

/**
 * @version 3.18.0
 */
struct ClientCodeLensResolveOptions {
        /**
         * The properties that a client can resolve lazily.
         */
        std::vector<string> properties;
};

struct CodeLensClientCapabilities {
        /**
         * Whether code lens supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Whether the client supports resolving additional code lens
         * properties via a separate @code codeLens/resolve@endcode request.
         *
         * @version 3.18.0
         */
        std::optional<ClientCodeLensResolveOptions> resolveSupport;
};

struct DocumentLinkClientCapabilities {
        /**
         * Whether document link supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Whether the client supports the @code tooltip@endcode property on @code DocumentLink@endcode.
         *
         * @version 3.15.0
         */
        std::optional<boolean> tooltipSupport;
};

struct DocumentColorClientCapabilities {
        /**
         * Whether document color supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;
};

struct DocumentFormattingClientCapabilities {
        /**
         * Whether formatting supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;
};

struct DocumentRangeFormattingClientCapabilities {
        /**
         * Whether formatting supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Whether the client supports formatting multiple ranges at once.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<boolean> rangesSupport;
};

struct DocumentOnTypeFormattingClientCapabilities {
        /**
         * Whether on type formatting supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;
};

enum class PrepareSupportDefaultBehavior {
        /**
         * The client's default behavior is to select the identifier
         * according to the language's syntax rule.
         */
        Identifier = 1

};

struct RenameClientCapabilities {
        /**
         * Whether rename supports dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Client supports testing for validity of rename operations
         * before execution.
         *
         * @version version 3.12.0
         */
        std::optional<boolean> prepareSupport;

        /**
         * Client supports the default behavior result
         * (@code { defaultBehavior: boolean }@endcode).
         *
         * The value indicates the default behavior used by the
         * client.
         *
         * @version version 3.16.0
         */
        std::optional<PrepareSupportDefaultBehavior> prepareSupportDefaultBehavior;

        /**
         * Whether the client honors the change annotations in
         * text edits and resource operations returned via the
         * rename request's workspace edit by for example presenting
         * the workspace edit in the user interface and asking
         * for confirmation.
         *
         * @version 3.16.0
         */
        std::optional<boolean> honorsChangeAnnotations;
};

/**
 * The diagnostic tags.
 *
 * @version 3.15.0
 */
enum class DiagnosticTag {
        /**
         * Unused or unnecessary code.
         *
         * Clients are allowed to render diagnostics with this tag faded out
         * instead of having an error squiggle.
         */
        Unnecessary = 1,
        /**
         * Deprecated or obsolete code.
         *
         * Clients are allowed to rendered diagnostics with this tag strike through.
         */
        Deprecated = 2

};

struct PublishDiagnosticsClientCapabilities {
        struct _TagSupport {
                /**
                 * The tags supported by the client.
                 */
                std::vector<DiagnosticTag> valueSet;
        };

        /**
         * Whether the clients accepts diagnostics with related information.
         */
        std::optional<boolean> relatedInformation;

        /**
         * Client supports the tag property to provide meta data about a diagnostic.
         * Clients supporting tags have to handle unknown tags gracefully.
         *
         * @version 3.15.0
         */
        std::optional<_TagSupport> tagSupport;

        /**
         * Whether the client interprets the version property of the
         * @code textDocument/publishDiagnostics@endcode notification's parameter.
         *
         * @version 3.15.0
         */
        std::optional<boolean> versionSupport;

        /**
         * Client supports a codeDescription property
         *
         * @version 3.16.0
         */
        std::optional<boolean> codeDescriptionSupport;

        /**
         * Whether code action supports the @code data@endcode property which is
         * preserved between a @code textDocument/publishDiagnostics@endcode and
         * @code textDocument/codeAction@endcode request.
         *
         * @version 3.16.0
         */
        std::optional<boolean> dataSupport;
};

STRING_ENUM(FoldingRangeKind,
        Comment, "comment",
        Imports, "imports",
        Region,  "region"
)

struct FoldingRangeClientCapabilities {
        struct _FoldingRangeKind {
                /**
                 * The folding range kind values the client supports. When this
                 * property exists the client also guarantees that it will
                 * handle values outside its set gracefully and falls back
                 * to a default value when unknown.
                 */
                std::optional<std::vector<FoldingRangeKind>> valueSet;
        };

        struct _FoldingRange {
                /**
                * If set, the client signals that it supports setting collapsedText on
                * folding ranges to display custom labels instead of the default text.
                *
                * @version 3.17.0
                */
                std::optional<boolean> collapsedText;
        };

        /**
         * Whether implementation supports dynamic registration for folding range
         * providers. If this is set to @code true@endcode the client supports the new
         * @code FoldingRangeRegistrationOptions@endcode return value for the corresponding
         * server capability as well.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The maximum number of folding ranges that the client prefers to receive
         * per document. The value serves as a hint, servers are free to follow the
         * limit.
         */
        std::optional<uinteger> rangeLimit;

        /**
         * If set, the client signals that it only supports folding complete lines.
         * If set, client will ignore specified @code startCharacter@endcode and @code endCharacter@endcode
         * properties in a FoldingRange.
         */
        std::optional<boolean> lineFoldingOnly;

        /**
         * Specific options for the folding range kind.
         *
         * @version 3.17.0
         */
        std::optional<_FoldingRangeKind> foldingRangeKind;

        /**
         * Specific options for the folding range.
         * @version 3.17.0
         */
        std::optional<_FoldingRange> foldingRange;
};

struct SelectionRangeClientCapabilities {
        /**
         * Whether implementation supports dynamic registration for selection range
         * providers. If this is set to @code true@endcode the client supports the new
         * @code SelectionRangeRegistrationOptions@endcode return value for the corresponding
         * server capability as well.
         */
        std::optional<boolean> dynamicRegistration;
};

struct LinkedEditingRangeClientCapabilities {
        /**
         * Whether the implementation supports dynamic registration.
         * If this is set to @code true@endcode the client supports the new
         * @code (TextDocumentRegistrationOptions & StaticRegistrationOptions)@endcode
         * return value for the corresponding server capability as well.
         */
        std::optional<boolean> dynamicRegistration;
};

struct CallHierarchyClientCapabilities {
        /**
         * Whether implementation supports dynamic registration. If this is set to
         * @code true@endcode the client supports the new @code (TextDocumentRegistrationOptions &
         * StaticRegistrationOptions)@endcode return value for the corresponding server
         * capability as well.
         */
        std::optional<boolean> dynamicRegistration;
};

STRING_ENUM(TokenFormat,
        Relative, "relative"
)

STRING_ENUM(SemanticTokenTypes,
        Namespace, "namespace",
        Type, "type",
        Class, "class",
        Enum, "enum",
        Interface, "interface",
        Struct, "struct",
        TypeParameter, "typeParameter",
        Parameter, "parameter",
        Variable, "variable",
        Property, "property",
        EnumMember, "enumMember",
        Event, "event",
        Function, "function",
        Method, "method",
        Macro, "macro",
        Keyword, "keyword",
        Modifier, "modifier",
        Comment, "comment",
        String, "string",
        Number, "number",
        Regexp, "regexp",
        Operator, "operator",
        Decorator, "decorator",
        Label, "label"
)

STRING_ENUM(SemanticTokenModifiers,
        Declaration, "declaration",
        Definition, "definition",
        Readonly, "readonly",
        Static, "static",
        Deprecated, "deprecated",
        Abstract, "abstract",
        Async, "async",
        Modification, "modification",
        Documentation, "documentation",
        DefaultLibrary, "defaultLibrary"
)

struct SemanticTokensClientCapabilities {
        /**
         * Whether implementation supports dynamic registration. If this is set to
         * @code true@endcode the client supports the new @code (TextDocumentRegistrationOptions &
         * StaticRegistrationOptions)@endcode return value for the corresponding server
         * capability as well.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Which requests the client supports and might send to the server
         * depending on the server's capability. Please note that clients might not
         * show semantic tokens or degrade some of the user experience if a range
         * or full request is advertised by the client but not provided by the
         * server. If for example the client capability @code requests.full@endcode and
         * @code request.range@endcode are both set to true but the server only provides a
         * range provider the client might not render a minimap correctly or might
         * even decide to not show any semantic tokens at all.
         */
        struct {
                struct _Range {
                };

                struct _Full {
                        /**
                         * The client will send the @code textDocument/semanticTokens/full/delta@endcode
                         * request if the server provides a corresponding handler.
                         */
                        std::optional<boolean> delta;
                };

                /**
                 * The client will send the @code textDocument/semanticTokens/range@endcode request
                 * if the server provides a corresponding handler.
                 */
                std::optional<std::variant<boolean, _Range>> range;

                /**
                 * The client will send the @code textDocument/semanticTokens/full@endcode request
                 * if the server provides a corresponding handler.
                 */
                std::optional<std::variant<boolean, _Full>> full;
        } requests;

        /**
         * The token types that the client supports.
         */
        std::vector<string> tokenTypes;

        /**
         * The token modifiers that the client supports.
         */
        std::vector<string> tokenModifiers;

        /**
         * The formats the clients supports.
         */
        std::vector<TokenFormat> formats;

        /**
         * Whether the client supports tokens that can overlap each other.
         */
        std::optional<boolean> overlappingTokenSupport;

        /**
         * Whether the client supports tokens that can span multiple lines.
         */
        std::optional<boolean> multilineTokenSupport;

        /**
         * Whether the client allows the server to actively cancel a
         * semantic token request, e.g. supports returning
         * ErrorCodes.ServerCancelled. If a server does the client
         * needs to retrigger the request.
         *
         * @version 3.17.0
         */
        std::optional<boolean> serverCancelSupport;

        /**
         * Whether the client uses semantic tokens to augment existing
         * syntax tokens. If set to @code true@endcode client side created syntax
         * tokens and semantic tokens are both used for colorization. If
         * set to @code false@endcode the client only uses the returned semantic tokens
         * for colorization.
         *
         * If the value is @code undefined@endcode then the client behavior is not
         * specified.
         *
         * @version 3.17.0
         */
        std::optional<boolean> augmentsSyntaxTokens;
};

struct MonikerClientCapabilities {
        /**
         * Whether implementation supports dynamic registration. If this is set to
         * @code true@endcode the client supports the new @code (TextDocumentRegistrationOptions &
         * StaticRegistrationOptions)@endcode return value for the corresponding server
         * capability as well.
         */
        std::optional<boolean> dynamicRegistration;
};

struct TypeHierarchyClientCapabilities {
        /**
         * Whether implementation supports dynamic registration. If this is set to
         * @code true@endcode the client supports the new @code (TextDocumentRegistrationOptions &
         * StaticRegistrationOptions)@endcode return value for the corresponding server
         * capability as well.
         */
        std::optional<boolean> dynamicRegistration;
};

/**
 * Client capabilities specific to inline values.
 *
 * @version 3.17.0
 */
struct InlineValueClientCapabilities {
        /**
         * Whether implementation supports dynamic registration for inline
         * value providers.
         */
        std::optional<boolean> dynamicRegistration;
};

/**
 * Inlay hint client capabilities.
 *
 * @version 3.17.0
 */
struct InlayHintClientCapabilities {
        struct _ResolveSupport {
                /**
                 * The properties that a client can resolve lazily.
                 */
                std::vector<string> properties;
        };

        /**
         * Whether inlay hints support dynamic registration.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Indicates which properties a client can resolve lazily on an inlay
         * hint.
         */
        std::optional<_ResolveSupport> resolveSupport;
};

/**
 * Client capabilities specific to diagnostic pull requests.
 *
 * @version 3.17.0
 */
struct DiagnosticClientCapabilities {
        /**
         * Whether implementation supports dynamic registration. If this is set to
         * @code true@endcode the client supports the new
         * @code (TextDocumentRegistrationOptions & StaticRegistrationOptions)@endcode
         * return value for the corresponding server capability as well.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * Whether the clients supports related documents for document diagnostic
         * pulls.
         */
        std::optional<boolean> relatedDocumentSupport;

        /**
         * Whether the client supports @code MarkupContent@endcode in diagnostic messages.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<boolean> markupMessageSupport;
};

struct TextDocumentFilterClientCapabilities {
        /**
         * The client supports Relative Patterns.
         *
         * @version 3.18.0
         */
        std::optional<boolean> relativePatternSupport;
};

/**
 * Client capabilities specific to inline completions.
 *
 * @version 3.18.0
 */
struct InlineCompletionClientCapabilities {
        /**
         * Whether implementation supports dynamic registration for inline
         * completion providers.
         */
        std::optional<boolean> dynamicRegistration;
};

/**
 * Text document specific client capabilities.
 */
struct TextDocumentClientCapabilities {
        std::optional<TextDocumentSyncClientCapabilities> synchronization;

        /**
         * Defines which filters the client supports.
         *
         * @version 3.18.0
         */
        std::optional<TextDocumentFilterClientCapabilities> filters;

        /**
         * Capabilities specific to the @code textDocument/completion@endcode request.
         */
        std::optional<CompletionClientCapabilities> completion;

        /**
         * Capabilities specific to the @code textDocument/hover@endcode request.
         */
        std::optional<HoverClientCapabilities> hover;

        /**
         * Capabilities specific to the @code textDocument/signatureHelp@endcode request.
         */
        std::optional<SignatureHelpClientCapabilities> signatureHelp;

        /**
         * Capabilities specific to the @code textDocument/declaration@endcode request.
         *
         * @version 3.14.0
         */
        std::optional<DeclarationClientCapabilities> declaration;

        /**
         * Capabilities specific to the @code textDocument/definition@endcode request.
         */
        std::optional<DefinitionClientCapabilities> definition;

        /**
         * Capabilities specific to the @code textDocument/typeDefinition@endcode request.
         *
         * @version 3.6.0
         */
        std::optional<TypeDefinitionClientCapabilities> typeDefinition;

        /**
         * Capabilities specific to the @code textDocument/implementation@endcode request.
         *
         * @version 3.6.0
         */
        std::optional<ImplementationClientCapabilities> implementation;

        /**
         * Capabilities specific to the @code textDocument/references@endcode request.
         */
        std::optional<ReferenceClientCapabilities> references;

        /**
         * Capabilities specific to the @code textDocument/documentHighlight@endcode request.
         */
        std::optional<DocumentHighlightClientCapabilities> documentHighlight;

        /**
         * Capabilities specific to the @code textDocument/documentSymbol@endcode request.
         */
        std::optional<DocumentSymbolClientCapabilities> documentSymbol;

        /**
         * Capabilities specific to the @code textDocument/codeAction@endcode request.
         */
        std::optional<CodeActionClientCapabilities> codeAction;

        /**
         * Capabilities specific to the @code textDocument/codeLens@endcode request.
         */
        std::optional<CodeLensClientCapabilities> codeLens;

        /**
         * Capabilities specific to the @code textDocument/documentLink@endcode request.
         */
        std::optional<DocumentLinkClientCapabilities> documentLink;

        /**
         * Capabilities specific to the @code textDocument/documentColor@endcode and the
         * @code textDocument/colorPresentation@endcode request.
         *
         * @version 3.6.0
         */
        std::optional<DocumentColorClientCapabilities> colorProvider;

        /**
         * Capabilities specific to the @code textDocument/formatting@endcode request.
         */
        std::optional<DocumentFormattingClientCapabilities> formatting;

        /**
         * Capabilities specific to the @code textDocument/rangeFormatting@endcode request.
         */
        std::optional<DocumentRangeFormattingClientCapabilities> rangeFormatting;

        /** request.
         * Capabilities specific to the @code textDocument/onTypeFormatting@endcode request.
         */
        std::optional<DocumentOnTypeFormattingClientCapabilities> onTypeFormatting;

        /**
         * Capabilities specific to the @code textDocument/rename@endcode request.
         */
        std::optional<RenameClientCapabilities> rename;

        /**
         * Capabilities specific to the @code textDocument/publishDiagnostics@endcode
         * notification.
         */
        std::optional<PublishDiagnosticsClientCapabilities> publishDiagnostics;

        /**
         * Capabilities specific to the @code textDocument/foldingRange@endcode request.
         *
         * @version 3.10.0
         */
        std::optional<FoldingRangeClientCapabilities> foldingRange;

        /**
         * Capabilities specific to the @code textDocument/selectionRange@endcode request.
         *
         * @version 3.15.0
         */
        std::optional<SelectionRangeClientCapabilities> selectionRange;

        /**
         * Capabilities specific to the @code textDocument/linkedEditingRange@endcode request.
         *
         * @version 3.16.0
         */
        std::optional<LinkedEditingRangeClientCapabilities> linkedEditingRange;

        /**
         * Capabilities specific to the various call hierarchy requests.
         *
         * @version 3.16.0
         */
        std::optional<CallHierarchyClientCapabilities> callHierarchy;

        /**
         * Capabilities specific to the various semantic token requests.
         *
         * @version 3.16.0
         */
        std::optional<SemanticTokensClientCapabilities> semanticTokens;

        /**
         * Capabilities specific to the @code textDocument/moniker@endcode request.
         *
         * @version 3.16.0
         */
        std::optional<MonikerClientCapabilities> moniker;

        /**
         * Capabilities specific to the various type hierarchy requests.
         *
         * @version 3.17.0
         */
        std::optional<TypeHierarchyClientCapabilities> typeHierarchy;

        /**
         * Capabilities specific to the @code textDocument/inlineValue@endcode request.
         *
         * @version 3.17.0
         */
        std::optional<InlineValueClientCapabilities> inlineValue;

        /**
         * Capabilities specific to the @code textDocument/inlayHint@endcode request.
         *
         * @version 3.17.0
         */
        std::optional<InlayHintClientCapabilities> inlayHint;

        /**
         * Capabilities specific to the diagnostic pull model.
         *
         * @version 3.17.0
         */
        std::optional<DiagnosticClientCapabilities> diagnostic;

        /**
         * Capabilities specific to the @code textDocument/inlineCompletion@endcode request.
         *
         * @version 3.18.0
         */
        std::optional<InlineCompletionClientCapabilities> inlineCompletion;
};

/**
 * Notebook specific client capabilities.
 *
 * @version 3.17.0
 */
struct NotebookDocumentSyncClientCapabilities {
        /**
         * Whether implementation supports dynamic registration. If this is
         * set to @code true@endcode the client supports the new
         * @code (NotebookDocumentSyncRegistrationOptions & NotebookDocumentSyncOptions)@endcode
         * return value for the corresponding server capability as well.
         */
        std::optional<boolean> dynamicRegistration;

        /**
         * The client supports sending execution summary data per cell.
         */
        std::optional<boolean> executionSummarySupport;
};

/**
 * Capabilities specific to the notebook document support.
 *
 * @version 3.17.0
 */
struct NotebookDocumentClientCapabilities {
        /**
         * Capabilities specific to notebook document synchronization
         *
         * @version 3.17.0
         */
        NotebookDocumentSyncClientCapabilities synchronization;
};

/**
 * Show message request client capabilities
 */
struct ShowMessageRequestClientCapabilities {
        struct _MessageActionItem {
                /**
                 * Whether the client supports additional attributes which
                 * are preserved and sent back to the server in the
                 * request's response.
                 */
                std::optional<boolean> additionalPropertiesSupport;
        };

        /**
         * Capabilities specific to the @code MessageActionItem@endcode type.
         */
        std::optional<_MessageActionItem> messageActionItem;
};

/**
 * Client capabilities for the show document request.
 *
 * @version 3.16.0
 */
struct ShowDocumentClientCapabilities {
        /**
         * The client has support for the show document
         * request.
         */
        std::optional<boolean> support;
};

/**
 * Client capabilities specific to regular expressions.
 */
struct RegularExpressionsClientCapabilities {
        /**
         * The engine's name.
         */
        string engine;

        /**
         * The engine's version.
         */
        std::optional<string> version;
};

/**
 * Client capabilities specific to the used markdown parser.
 *
 * @version 3.16.0
 */
struct MarkdownClientCapabilities {
        /**
         * The name of the parser.
         */
        string parser;

        /**
         * The version of the parser.
         */
        std::optional<string> version;

        /**
         * A list of HTML tags that the client allows / supports in
         * Markdown.
         *
         * @version 3.17.0
         */
        std::optional<std::vector<string>> allowedTags;
};

/**
 * A type indicating how positions are encoded,
 * specifically what column offsets mean.
 *
 * @version 3.17.0
 */
STRING_ENUM(PositionEncodingKind,
        UTF8, "utf-8",
        UTF16, "utf-16",
        UTF32, "utf-32"
)

struct ClientCapabilities {
        struct _Workspace {
                struct _FileOperations {
                        /**
                         * Whether the client supports dynamic registration for file
                         * requests/notifications.
                         */
                        std::optional<boolean> dynamicRegistration;

                        /**
                         * The client has support for sending didCreateFiles notifications.
                         */
                        std::optional<boolean> didCreate;

                        /**
                         * The client has support for sending willCreateFiles requests.
                         */
                        std::optional<boolean> willCreate;

                        /**
                         * The client has support for sending didRenameFiles notifications.
                         */
                        std::optional<boolean> didRename;

                        /**
                         * The client has support for sending willRenameFiles requests.
                         */
                        std::optional<boolean> willRename;

                        /**
                         * The client has support for sending didDeleteFiles notifications.
                         */
                        std::optional<boolean> didDelete;

                        /**
                         * The client has support for sending willDeleteFiles requests.
                         */
                        std::optional<boolean> willDelete;
                };

                /**
                 * The client supports applying batch edits
                 * to the workspace by supporting the request
                 * 'workspace/applyEdit'
                 */
                std::optional<boolean> applyEdit;

                /**
                 * Capabilities specific to @code WorkspaceEdit@endcode s
                 */
                std::optional<WorkspaceEditClientCapabilities> workspaceEdit;

                /**
                 * Capabilities specific to the @code workspace/didChangeConfiguration@endcode
                 * notification.
                 */
                std::optional<DidChangeConfigurationClientCapabilities> didChangeConfiguration;

                /**
                 * Capabilities specific to the @code workspace/didChangeWatchedFiles@endcode
                 * notification.
                 */
                std::optional<DidChangeWatchedFilesClientCapabilities> didChangeWatchedFiles;

                /**
                 * Capabilities specific to the @code workspace/symbol@endcode request.
                 */
                std::optional<WorkspaceSymbolClientCapabilities> symbol;

                /**
                 * Capabilities specific to the @code workspace/executeCommand@endcode request.
                 */
                std::optional<ExecuteCommandClientCapabilities> executeCommand;

                /**
                 * The client has support for workspace folders.
                 *
                 * @version 3.6.0
                 */
                std::optional<boolean> workspaceFolders;

                /**
                 * The client supports @code workspace/configuration@endcode requests.
                 *
                 * @version 3.6.0
                 */
                std::optional<boolean> configuration;

                /**
                 * Capabilities specific to the semantic token requests scoped to the
                 * workspace.
                 *
                 * @version 3.16.0
                 */
                std::optional<SemanticTokensWorkspaceClientCapabilities> semanticTokens;

                /**
                 * Capabilities specific to the code lens requests scoped to the
                 * workspace.
                 *
                 * @version 3.16.0
                 */
                std::optional<CodeLensWorkspaceClientCapabilities> codeLens;

                /**
                 * The client has support for file requests/notifications.
                 *
                 * @version 3.16.0
                 */
                std::optional<_FileOperations> fileOperations;

                /**
                 * Client workspace capabilities specific to inline values.
                 *
                 * @version 3.17.0
                 */
                std::optional<InlineValueWorkspaceClientCapabilities> inlineValue;

                /**
                 * Client workspace capabilities specific to inlay hints.
                 *
                 * @version 3.17.0
                 */
                std::optional<InlayHintWorkspaceClientCapabilities> inlayHint;

                /**
                 * Client workspace capabilities specific to diagnostics.
                 *
                 * @version 3.17.0.
                 */
                std::optional<DiagnosticWorkspaceClientCapabilities> diagnostics;

                // TODO not added in the ClientCapabilities.workspace documentation,
                //  but method exists
                std::optional<TextDocumentContentClientCapabilities> textDocumentContent;

                // TODO not added in the ClientCapabilities.workspace documentation,
                //  but method exists
                std::optional<FoldingRangeWorkspaceClientCapabilities> foldingRange;
        };

        struct _Window {
                /**
                 * It indicates whether the client supports server initiated
                 * progress using the @code window/workDoneProgress/create@endcode request.
                 *
                 * The capability also controls Whether client supports handling
                 * of progress notifications. If set servers are allowed to report a
                 * @code workDoneProgress@endcode property in the request specific server
                 * capabilities.
                 *
                 * @version 3.15.0
                 */
                std::optional<boolean> workDoneProgress;

                /**
                 * Capabilities specific to the showMessage request
                 *
                 * @version 3.16.0
                 */
                std::optional<ShowMessageRequestClientCapabilities> showMessage;

                /**
                 * Client capabilities for the show document request.
                 *
                 * @version 3.16.0
                 */
                std::optional<ShowDocumentClientCapabilities> showDocument;
        };

        struct _General {
                struct _StaleRequestSupport {
                        /**
                         * The client will actively cancel the request.
                         */
                        boolean cancel;

                        /**
                         * The list of requests for which the client
                         * will retry the request if it receives a
                         * response with error code @code ContentModified@endcode
                         */
                        std::vector<string> retryOnContentModified;
                };

                /**
                 * Client capability that signals how the client
                 * handles stale requests (e.g. a request
                 * for which the client will not process the response
                 * anymore since the information is outdated).
                 *
                 * @version 3.17.0
                 */
                std::optional<_StaleRequestSupport> staleRequestSupport;

                /**
                 * Client capabilities specific to regular expressions.
                 *
                 * @version 3.16.0
                 */
                std::optional<RegularExpressionsClientCapabilities> regularExpressions;

                /**
                 * Client capabilities specific to the client's markdown parser.
                 *
                 * @version 3.16.0
                 */
                std::optional<MarkdownClientCapabilities> markdown;

                /**
                 * The position encodings supported by the client. Client and server
                 * have to agree on the same position encoding to ensure that offsets
                 * (e.g. character position in a line) are interpreted the same on both
                 * side.
                 *
                 * To keep the protocol backwards compatible the following applies: if
                 * the value 'utf-16' is missing from the array of position encodings
                 * servers can assume that the client supports UTF-16. UTF-16 is
                 * therefore a mandatory encoding.
                 *
                 * If omitted it defaults to ['utf-16'].
                 *
                 * Implementation considerations: since the conversion from one encoding
                 * into another requires the content of the file / line the conversion
                 * is best done where the file is read which is usually on the server
                 * side.
                 *
                 * @version 3.17.0
                 */
                std::optional<std::vector<PositionEncodingKind>> positionEncodings;
        };

        /**
         * Workspace specific client capabilities.
         */
        std::optional<_Workspace> workspace;

        /**
         * Text document specific client capabilities.
         */
        std::optional<TextDocumentClientCapabilities> textDocument;

        /**
         * Capabilities specific to the notebook document support.
         *
         * @version 3.17.0
         */
        std::optional<NotebookDocumentClientCapabilities> notebookDocument;

        /**
         * Window specific client capabilities.
         */
        std::optional<_Window> window;

        /**
         * General client capabilities.
         *
         * @version 3.16.0
         */
        std::optional<_General> general;

        /**
         * Experimental client capabilities.
         */
        std::optional<LSPAny> experimental;
};

STRING_ENUM(TraceValue,
        off, "off",
        messages, "messages",
        verbose, "verbose"
)

/**
 * A tagging type for string properties that are actually document URIs.
 */
using DocumentUri = string;

/**
 * A tagging type for string properties that are actually URIs
 *
 * @version 3.16.0
 */
using URI = string;

struct WorkspaceFolder {
        /**
         * The associated URI for this workspace folder.
         */
        URI uri;

        /**
         * The name of the workspace folder. Used to refer to this
         * workspace folder in the user interface.
         */
        string name;
};

using ProgressToken = std::variant<integer, string>;

struct WorkDoneProgressParams {
        /**
         * An optional token that a server can use to report work done progress.
         */
        std::optional<ProgressToken> workDoneToken;
};

struct InitializeParams : WorkDoneProgressParams {
        struct _ClientInfo {
                /**
                 * The name of the client as defined by the client.
                 */
                string name;

                /**
                 * The client's version as defined by the client.
                 */
                std::optional<string> version;
        };

        /**
         * The process Id of the parent process that started the server. Is null if
         * the process has not been started by another process. If the parent
         * process is not alive then the server should exit (see exit notification)
         * its process.
         */
        std::variant<null, integer> processId;

        /**
         * Information about the client
         *
         * @version 3.15.0
         */
        std::optional<_ClientInfo> clientInfo;

        /**
         * The locale the client is currently showing the user interface
         * in. This must not necessarily be the locale of the operating
         * system.
         *
         * Uses IETF language tags as the value's syntax
         * (See https://en.wikipedia.org/wiki/IETF_language_tag)
         *
         * @version 3.16.0
         */
        std::optional<string> locale;

        /**
         * The rootPath of the workspace. Is null
         * if no folder is open.
         *
         * @deprecated in favour of @code rootUri@endcode.
         */
        std::optional<std::variant<null, string>> rootPath;

        /**
         * The rootUri of the workspace. Is null if no
         * folder is open. If both @code rootPath@endcode and @code rootUri@endcode are set
         * @code rootUri@endcode wins.
         *
         * @deprecated in favour of @code workspaceFolders@endcode
         */
        std::optional<std::variant<null, DocumentUri>> rootUri;

        /**
         * User provided initialization options.
         */
        std::optional<LSPAny> initializationOptions;

        /**
         * The capabilities provided by the client (editor or tool)
         */
        ClientCapabilities capabilities;

        /**
         * The initial trace setting. If omitted trace is disabled ('off').
         */
        std::optional<TraceValue> trace;

        /**
         * The workspace folders configured in the client when the server starts.
         * This property is only available if the client supports workspace folders.
         * It can be @code null@endcode if the client supports workspace folders but none are
         * configured.
         *
         * @version 3.6.0
         */
        std::optional<std::variant<null, std::vector<WorkspaceFolder>>> workspaceFolders;
};

/**
 * Defines how the host (editor) should sync document changes to the language
 * server.
 */
enum class TextDocumentSyncKind {
        /**
         * Documents should not be synced at all.
         */
        None = 0,

        /**
         * Documents are synced by always sending the full content
         * of the document.
         */
        Full = 1,

        /**
         * Documents are synced by sending the full content on open.
         * After that only incremental updates to the document are
         * sent.
         */
        Incremental = 2

};

struct TextDocumentSyncOptions {
        /**
         * Open and close notifications are sent to the server. If omitted open
         * close notifications should not be sent.
         */
        std::optional<boolean> openClose;

        /**
         * Change notifications are sent to the server. See
         * TextDocumentSyncKind.None, TextDocumentSyncKind.Full and
         * TextDocumentSyncKind.Incremental. If omitted it defaults to
         * TextDocumentSyncKind.None.
         */
        std::optional<TextDocumentSyncKind> change;
};

struct _NotebookDocumentFilter1 {
        /** The type of the enclosing notebook. */
        string notebookType;

        /** A Uri scheme, like @code file@endcode or @code untitled@endcode. */
        std::optional<string> scheme;

        /** A glob pattern. */
        std::optional<string> pattern;
};

struct _NotebookDocumentFilter2 {
        /** The type of the enclosing notebook. */
        std::optional<string> notebookType;

        /** A Uri scheme, like @code file@endcode or @code untitled@endcode. */
        string scheme;

        /** A glob pattern. */
        std::optional<string> pattern;
};

struct _NotebookDocumentFilter3 {
        /** The type of the enclosing notebook. */
        std::optional<string> notebookType;

        /** A Uri scheme, like @code file@endcode or @code untitled@endcode. */
        std::optional<string> scheme;

        /** A glob pattern. */
        string pattern;
};

/**
 * A notebook document filter denotes a notebook document by
 * different properties.
 *
 * @version 3.17.0
 */
using NotebookDocumentFilter = std::variant<_NotebookDocumentFilter1, _NotebookDocumentFilter2, _NotebookDocumentFilter3>;

/**
 * Options specific to a notebook plus its cells
 * to be synced to the server.
 *
 * If a selector provides a notebook document
 * filter but no cell selector all cells of a
 * matching notebook document will be synced.
 *
 * If a selector provides no notebook document
 * filter but only a cell selector all notebook
 * documents that contain at least one matching
 * cell will be synced.
 *
 * @version 3.17.0
 */
struct NotebookDocumentSyncOptions {
        struct _Cell {
                string language;
        };

        struct _NotebookSelector1 {
                /**
                 * The notebook to be synced. If a string
                 * value is provided it matches against the
                 * notebook type. '*' matches every notebook.
                 */
                std::variant<string, NotebookDocumentFilter> notebook;

                /**
                 * The cells of the matching notebook to be synced.
                 */
                std::optional<std::optional<_Cell>> cells;
        };

        struct _NotebookSelector2 {
                /**
                 * The notebook to be synced. If a string
                 * value is provided it matches against the
                 * notebook type. '*' matches every notebook.
                 */
                std::optional<std::variant<string, NotebookDocumentFilter>> notebook;

                /**
                 * The cells of the matching notebook to be synced.
                 */
                std::optional<_Cell> cells;
        };

        using _NotebookSelector = std::variant<_NotebookSelector1, _NotebookSelector2>;

        /**
         * The notebooks to be synced
         */
        std::vector<_NotebookSelector> notebookSelector;

        /**
         * Whether save notification should be forwarded to
         * the server. Will only be honored if mode === @code notebook@endcode.
         */
        std::optional<boolean> save;
};

/**
 * Static registration options to be returned in the initialize request.
 */
struct StaticRegistrationOptions {
        /**
         * The id used to register the request. The id can be used to deregister
         * the request again. See also Registration#id.
         */
        std::optional<string> id;
};

/**
 * Registration options specific to a notebook.
 *
 * @version 3.17.0
 */
struct NotebookDocumentSyncRegistrationOptions : NotebookDocumentSyncOptions, StaticRegistrationOptions {
};

struct WorkDoneProgressOptions {
        std::optional<boolean> workDoneProgress;
};

/**
 * Completion options.
 */
struct CompletionOptions : WorkDoneProgressOptions {
        struct _CompletionItem {
                /**
                 * The server has support for completion item label
                 * details (see also @code CompletionItemLabelDetails@endcode) when receiving
                 * a completion item in a resolve call.
                 *
                 * @version 3.17.0
                 */
                std::optional<boolean> labelDetailsSupport;
        };

        /**
         * The additional characters, beyond the defaults provided by the client (typically
         * [a-zA-Z]), that should automatically trigger a completion request. For example
         * @code .@endcode in JavaScript represents the beginning of an object property or method and is
         * thus a good candidate for triggering a completion request.
         *
         * Most tools trigger a completion request automatically without explicitly
         * requesting it using a keyboard shortcut (e.g. Ctrl+Space). Typically they
         * do so when the user starts to type an identifier. For example if the user
         * types @code c@endcode in a JavaScript file code complete will automatically pop up
         * present @code console@endcode besides others as a completion item. Characters that
         * make up identifiers don't need to be listed here.
         */
        std::optional<std::vector<string>> triggerCharacters;

        /**
         * The list of all possible characters that commit a completion. This field
         * can be used if clients don't support individual commit characters per
         * completion item. See client capability
         * @code completion.completionItem.commitCharactersSupport@endcode.
         *
         * If a server provides both @code allCommitCharacters@endcode and commit characters on
         * an individual completion item the ones on the completion item win.
         *
         * @version 3.2.0
         */
        std::optional<std::vector<string>> allCommitCharacters;

        /**
         * The server provides support to resolve additional
         * information for a completion item.
         */
        std::optional<boolean> resolveProvider;

        /**
         * The server supports the following @code CompletionItem@endcode specific
         * capabilities.
         *
         * @version 3.17.0
         */
        std::optional<_CompletionItem> completionItem;
};

struct HoverOptions : WorkDoneProgressOptions {
};

struct SignatureHelpOptions : WorkDoneProgressOptions {
        /**
         * The characters that trigger signature help
         * automatically.
         */
        std::optional<std::vector<string>> triggerCharacters;

        /**
         * List of characters that re-trigger signature help.
         *
         * These trigger characters are only active when signature help is already
         * showing. All trigger characters are also counted as re-trigger
         * characters.
         *
         * @version 3.15.0
         */
        std::optional<std::vector<string>> retriggerCharacters;
};

struct DeclarationOptions : WorkDoneProgressOptions {
};

struct DocumentFilter {
        /**
         * A language id, like @code typescript@endcode.
         */
        std::optional<string> language;

        /**
         * A Uri scheme, like @code file@endcode or @code untitled@endcode.
         */
        std::optional<string> scheme;

        /**
         * A glob pattern, like @code *.{ts,js}@endcode.
         *
         * Glob patterns can have the following syntax:
         * - @code *@endcode to match zero or more characters in a path segment
         * - @code ?@endcode to match on one character in a path segment
         * - @code **@endcode to match any number of path segments, including none
         * - @code {}@endcode to group sub patterns into an OR expression. (e.g. @code **∕*.{ts,js}@endcode
         *   matches all TypeScript and JavaScript files)
         * - @code []@endcode to declare a range of characters to match in a path segment
         *   (e.g., @code example.[0-9]@endcode to match on @code example.0@endcode, @code example.1@endcode, …)
         * - @code [!...]@endcode to negate a range of characters to match in a path segment
         *   (e.g., @code example.[!0-9]@endcode to match on @code example.a@endcode, @code example.b@endcode, but
         *   not @code example.0@endcode)
         */
        std::optional<string> pattern;
};

using DocumentSelector = std::vector<DocumentFilter>;

/**
 * General text document registration options.
 */
struct TextDocumentRegistrationOptions {
        /**
         * A document selector to identify the scope of the registration. If set to
         * null the document selector provided on the client side will be used.
         */
        std::variant<null, DocumentSelector> documentSelector;
};

struct DeclarationRegistrationOptions : DeclarationOptions, TextDocumentRegistrationOptions, StaticRegistrationOptions {
};

struct DefinitionOptions : WorkDoneProgressOptions {
};

struct TypeDefinitionOptions : WorkDoneProgressOptions {
};

struct TypeDefinitionRegistrationOptions : TextDocumentRegistrationOptions, TypeDefinitionOptions, StaticRegistrationOptions {
};

struct ImplementationOptions : WorkDoneProgressOptions {
};

struct ImplementationRegistrationOptions : TextDocumentRegistrationOptions, ImplementationOptions, StaticRegistrationOptions {
};

struct ReferenceOptions : WorkDoneProgressOptions {
};

struct DocumentHighlightOptions : WorkDoneProgressOptions {
};

struct DocumentSymbolOptions : WorkDoneProgressOptions {
        /**
         * A human-readable string that is shown when multiple outlines trees
         * are shown for the same document.
         *
         * @version 3.16.0
         */
        std::optional<string> label;
};

struct Command {
        /**
         * Title of the command, like @code save@endcode.
         */
        string title;

        /**
         * The identifier of the actual command handler.
         */
        string command;

        /**
         * Arguments that the command handler should be
         * invoked with.
         */
        std::optional<std::vector<LSPAny>> arguments;
};

/**
 * Documentation for a class of code actions.
 *
 * @version 3.18.0
 * @proposed
 */
struct CodeActionKindDocumentation {
        /**
         * The kind of the code action being documented.
         *
         * If the kind is generic, such as @code CodeActionKind.Refactor@endcode, the
         * documentation will be shown whenever any refactorings are returned. If
         * the kind is more specific, such as @code CodeActionKind.RefactorExtract@endcode, the
         * documentation will only be shown when extract refactoring code actions
         * are returned.
         */
        CodeActionKind kind;

        /**
         * Command that is used to display the documentation to the user.
         *
         * The title of this documentation code action is taken
         * from {@link Command::title @endlink}
         */
        Command command;
};

struct CodeActionOptions : WorkDoneProgressOptions {
        /**
         * CodeActionKinds that this server may return.
         *
         * The list of kinds may be generic, such as @code CodeActionKind.Refactor@endcode,
         * or the server may list out every specific kind they provide.
         */
        std::optional<std::vector<CodeActionKind>> codeActionKinds;

        /**
         * Static documentation for a class of code actions.
         *
         * Documentation from the provider should be shown in the code actions
         * menu if either:
         *
         * - Code actions of @code kind@endcode are requested by the editor. In this case,
         *   the editor will show the documentation that most closely matches the
         *   requested code action kind. For example, if a provider has
         *   documentation for both @code Refactor@endcode and @code RefactorExtract@endcode , when the
         *   user requests code actions for @code RefactorExtract@endcode, the editor will use
         *   the documentation for @code RefactorExtract@endcode instead of the documentation
         *   for @code Refactor@endcode.
         *
         * - Any code actions of @code kind@endcode are returned by the provider.
         *
         * At most one documentation entry should be shown per provider.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<std::vector<CodeActionKindDocumentation>> documentation;

        /**
         * The server provides support to resolve additional
         * information for a code action.
         *
         * @version 3.16.0
         */
        std::optional<boolean> resolveProvider;
};

struct CodeLensOptions : WorkDoneProgressOptions {
        /**
         * Code lens has a resolve provider as well.
         */
        std::optional<boolean> resolveProvider;
};

struct DocumentLinkOptions : WorkDoneProgressOptions {
        /**
         * Document links have a resolve provider as well.
         */
        std::optional<boolean> resolveProvider;
};

struct DocumentColorOptions : WorkDoneProgressOptions {
};

struct DocumentColorRegistrationOptions : TextDocumentRegistrationOptions, StaticRegistrationOptions, DocumentColorOptions {
};

struct DocumentFormattingOptions : WorkDoneProgressOptions {
};

struct DocumentRangeFormattingOptions : WorkDoneProgressOptions {
        /**
         * Whether the server supports formatting multiple ranges at once.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<boolean> rangesSupport;
};

struct DocumentOnTypeFormattingOptions {
        /**
         * A character on which formatting should be triggered, like @code {@endcode.
         */
        string firstTriggerCharacter;

        /**
         * More trigger characters.
         */
        std::optional<std::vector<string>> moreTriggerCharacters;
};

struct RenameOptions : WorkDoneProgressOptions {
        /**
         * Renames should be checked and tested before being executed.
         */
        std::optional<boolean> prepareProvider;
};

struct FoldingRangeOptions : WorkDoneProgressOptions {
};

struct FoldingRangeRegistrationOptions : TextDocumentRegistrationOptions, FoldingRangeOptions, StaticRegistrationOptions {
};

struct ExecuteCommandOptions : WorkDoneProgressOptions {
        /**
         * The commands to be executed on the server
         */
        std::vector<string> commands;
};

struct SelectionRangeOptions : WorkDoneProgressOptions {
};

struct SelectionRangeRegistrationOptions : SelectionRangeOptions, TextDocumentRegistrationOptions, StaticRegistrationOptions {
};

struct LinkedEditingRangeOptions : WorkDoneProgressOptions {
};

struct LinkedEditingRangeRegistrationOptions : TextDocumentRegistrationOptions, LinkedEditingRangeOptions, StaticRegistrationOptions {
};

struct CallHierarchyOptions : WorkDoneProgressOptions {
};

struct CallHierarchyRegistrationOptions : TextDocumentRegistrationOptions, CallHierarchyOptions, StaticRegistrationOptions {
};

struct SemanticTokensLegend {
        /**
         * The token types a server uses.
         */
        std::vector<string> tokenTypes;

        /**
         * The token modifiers a server uses.
         */
        std::vector<string> tokenModifiers;
};

struct SemanticTokensOptions : WorkDoneProgressOptions {
        struct _Empty {
        };

        struct _Full {
                /**
                 * The server supports deltas for full documents.
                 */
                std::optional<boolean> delta;
        };

        /**
         * The legend used by the server
         */
        SemanticTokensLegend legend;

        /**
         * Server supports providing semantic tokens for a specific range
         * of a document.
         */
        std::optional<std::variant<boolean, _Empty>> range;

        /**
         * Server supports providing semantic tokens for a full document.
         */
        std::optional<std::variant<boolean, _Full>> full;
};

struct SemanticTokensRegistrationOptions : TextDocumentRegistrationOptions, SemanticTokensOptions, StaticRegistrationOptions {
};

struct MonikerOptions : WorkDoneProgressOptions {
};

struct MonikerRegistrationOptions : TextDocumentRegistrationOptions, MonikerOptions {
};

struct TypeHierarchyOptions : WorkDoneProgressOptions {
};

struct TypeHierarchyRegistrationOptions : TextDocumentRegistrationOptions, TypeHierarchyOptions, StaticRegistrationOptions {
};

/**
 * Inline value options used during static registration.
 *
 * @version 3.17.0
 */
struct InlineValueOptions : WorkDoneProgressOptions {
};

/**
 * Inline value options used during static or dynamic registration.
 *
 * @version 3.17.0
 */
struct InlineValueRegistrationOptions : InlineValueOptions, TextDocumentRegistrationOptions, StaticRegistrationOptions {
};

/**
 * Inlay hint options used during static registration.
 *
 * @version 3.17.0
 */
struct InlayHintOptions : WorkDoneProgressOptions {
        /**
         * The server provides support to resolve additional
         * information for an inlay hint item.
         */
        std::optional<boolean> resolveProvider;
};

/**
 * Inlay hint options used during static or dynamic registration.
 *
 * @version 3.17.0
 */
struct InlayHintRegistrationOptions : InlayHintOptions, TextDocumentRegistrationOptions, StaticRegistrationOptions {
};

/**
 * Diagnostic options.
 *
 * @version 3.17.0
 */
struct DiagnosticOptions : WorkDoneProgressOptions {
        /**
         * An optional identifier under which the diagnostics are
         * managed by the client.
         */
        std::optional<string> identifier;

        /**
         * Whether the language has inter file dependencies meaning that
         * editing code in one file can result in a different diagnostic
         * set in another file. Inter file dependencies are common for
         * most programming languages and typically uncommon for linters.
         */
        boolean internalDependencies;

        /**
         * The server provides support for workspace diagnostics as well.
         */
        boolean workspaceDiagnostics;
};

/**
 * Diagnostic registration options.
 *
 * @version 3.17.0
 */
struct DiagnosticRegistrationOptions : TextDocumentRegistrationOptions, DiagnosticOptions, StaticRegistrationOptions {
};

struct WorkspaceSymbolOptions : WorkDoneProgressOptions {
        /**
         * The server provides support to resolve additional
         * information for a workspace symbol.
         *
         * @version 3.17.0
         */
        std::optional<boolean> resolveProvider;
};

struct WorkspaceFoldersServerCapabilities {
        /**
         * The server has support for workspace folders
         */
        std::optional<boolean> supported;

        /**
         * Whether the server wants to receive workspace folder
         * change notifications.
         *
         * If a string is provided, the string is treated as an ID
         * under which the notification is registered on the client
         * side. The ID can be used to unregister for these events
         * using the @code client/unregisterCapability@endcode request.
         */
        std::optional<std::variant<string, boolean>> changeNotifications;
};

/**
 * Text document content provider options.
 *
 * @version 3.18.0
 */
struct TextDocumentContentOptions {
        /**
         * The schemes for which the server provides content.
         */
        std::vector<string> schemes;
};

/**
 * Text document content provider registration options.
 *
 * @version 3.18.0
 */
struct TextDocumentContentRegistrationOptions : TextDocumentContentOptions, StaticRegistrationOptions {
};

/**
 * A pattern kind describing if a glob pattern matches a file a folder or
 * both.
 *
 * @version 3.16.0
 */
STRING_ENUM(FileOperationPatternKind,
        file, "file",
        folder, "folder"
);

/**
 * Matching options for the file operation pattern.
 *
 * @version 3.16.0
 */
struct FileOperationPatternOptions {
        /**
         * The pattern should be matched ignoring casing.
         */
        std::optional<boolean> ignoreCase;
};

/**
 * A pattern to describe in which file operation requests or notifications
 * the server is interested in.
 *
 * @version 3.16.0
 */
struct FileOperationPattern {
        /**
         * The glob pattern to match. Glob patterns can have the following syntax:
         * - @code *@endcode to match zero or more characters in a path segment
         * - @code ?@endcode to match on one character in a path segment
         * - @code **@endcode to match any number of path segments, including none
         * - @code {}@endcode to group sub patterns into an OR expression. (e.g. @code **∕*.{ts,js} @endcode
         *   matches all TypeScript and JavaScript files)
         * - @code []@endcode to declare a range of characters to match in a path segment
         *   (e.g., @code example.[0-9]@endcode to match on @code example.0@endcode, @code example.1@endcode, …)
         * - @code [!...]@endcode to negate a range of characters to match in a path segment
         *   (e.g., @code example.[!0-9]@endcode to match on @code example.a@endcode, @code example.b@endcode, but
         *   not @code example.0@endcode)
         */
        string glob;

        /**
         * Whether to match files or folders with this pattern.
         *
         * Matches both if undefined.
         */
        std::optional<FileOperationPatternKind> matches;

        /**
         * Additional options used during matching.
         */
        std::optional<FileOperationPatternOptions> options;
};

/**
 * A filter to describe in which file operation requests or notifications
 * the server is interested in.
 *
 * @version 3.16.0
 */
struct FileOperationFilter {
        /**
         * A Uri like @code file@endcode or @code untitled@endcode.
         */
        std::optional<string> scheme;

        /**
         * The actual file operation pattern.
         */
        FileOperationPattern pattern;
};

/**
 * The options to register for file operations.
 *
 * @version 3.16.0
 */
struct FileOperationRegistrationOptions {
        /**
         * The actual filters.
         */
        std::vector<FileOperationFilter> filters;
};

/**
 * Inline completion options used during static registration.
 *
 * @version 3.18.0
 */
struct InlineCompletionOptions : WorkDoneProgressOptions {
};

/**
 * Inline completion options used during static or dynamic registration.
 *
 * @version 3.18.0
 */
struct InlineCompletionRegistrationOptions : InlineCompletionOptions, TextDocumentRegistrationOptions, StaticRegistrationOptions {
};

struct ServerCapabilities {
        struct _Workspace {
                struct _FileOperation {
                        /**
                         * The server is interested in receiving didCreateFiles
                         * notifications.
                         */
                        FileOperationRegistrationOptions didCreate;

                        /**
                         * The server is interested in receiving willCreateFiles requests.
                         */
                        FileOperationRegistrationOptions willCreate;

                        /**
                         * The server is interested in receiving didRenameFiles
                         * notifications.
                         */
                        FileOperationRegistrationOptions didRename;

                        /**
                         * The server is interested in receiving willRenameFiles requests.
                         */
                        FileOperationRegistrationOptions willRename;

                        /**
                         * The server is interested in receiving didDeleteFiles file
                         * notifications.
                         */
                        FileOperationRegistrationOptions didDelete;

                        /**
                         * The server is interested in receiving willDeleteFiles file
                         * requests.
                         */
                        FileOperationRegistrationOptions willDelete;
                };

                /**
                 * The server supports workspace folder.
                 *
                 * @version 3.6.0
                 */
                std::optional<WorkspaceFoldersServerCapabilities> workspaceFolders;

                /**
                 * The server is interested in file notifications/requests.
                 *
                 * @version 3.16.0
                 */
                std::optional<_FileOperation> fileOperations;

                // TODO not in the ServerCapabilities.workspace documentation
                //  but method exists
                std::optional<TextDocumentContentOptions> textDocumentContent;
        };

        struct _TextDocument {
                struct _Diagnostic {
                        /**
                         * Whether the server supports @code MarkupContent@endcode in diagnostic messages.
                         *
                         * @version 3.18.0
                         * @proposed
                         */
                        std::optional<boolean> markupMessageSupport;
                };

                /**
                 * Capabilities specific to the diagnostic pull model.
                 *
                 * @version 3.18.0
                 */
                std::optional<_Diagnostic> diagnostic;
        };

        /**
         * The position encoding the server picked from the encodings offered
         * by the client via the client capability @code general.positionEncodings@endcode.
         *
         * If the client didn't provide any position encodings the only valid
         * value that a server can return is 'utf-16'.
         *
         * If omitted it defaults to 'utf-16'.
         *
         * @version 3.17.0
         */
        std::optional<PositionEncodingKind> positionEncoding;

        /**
         * Defines how text documents are synced. Is either a detailed structure
         * defining each notification or for backwards compatibility the
         * TextDocumentSyncKind number. If omitted it defaults to
         * @code TextDocumentSyncKind.None@endcode.
         */
        std::optional<std::variant<TextDocumentSyncOptions, TextDocumentSyncKind>> textDocumentSync;

        /**
         * Defines how notebook documents are synced.
         *
         * @version 3.17.0
         */
        std::optional<std::variant<NotebookDocumentSyncOptions, NotebookDocumentSyncRegistrationOptions>> notebookDocumentSync;

        /**
         * The server provides completion support.
         */
        std::optional<CompletionOptions> completionProvider;

        /**
	 * The server provides hover support.
	 */
        std::optional<std::variant<boolean, HoverOptions>> hoverProvider;

        /**
         * The server provides signature help support.
         */
        std::optional<SignatureHelpOptions> signatureHelpProvider;

        /**
         * The server provides go to declaration support.
         *
         * @version 3.14.0
         */
        std::optional<std::variant<boolean, DeclarationOptions, DeclarationRegistrationOptions>> declarationProvider;

        /**
         * The server provides goto definition support.
         */
        std::optional<std::variant<boolean, DefinitionOptions>> definitionProvider;

        /**
         * The server provides goto type definition support.
         *
         * @version 3.6.0
         */
        std::optional<std::variant<boolean, TypeDefinitionOptions, TypeDefinitionRegistrationOptions>> typeDefinitionProvider;

        /**
         * The server provides goto implementation support.
         *
         * @version 3.6.0
         */
        std::optional<std::variant<boolean, ImplementationOptions, ImplementationRegistrationOptions>> implementationProvider;

        /**
         * The server provides find references support.
         */
        std::optional<std::variant<boolean, ReferenceOptions>> referencesProvider;

        /**
         * The server provides document highlight support.
         */
        std::optional<std::variant<boolean, DocumentHighlightOptions>> documentHighlightProvider;

        /**
         * The server provides document symbol support.
         */
        std::optional<std::variant<boolean, DocumentSymbolOptions>> documentSymbolProvider;

        /**
         * The server provides code actions. The @code CodeActionOptions@endcode return type is
         * only valid if the client signals code action literal support via the
         * property @code textDocument.codeAction.codeActionLiteralSupport@endcode.
         */
        std::optional<std::variant<boolean, CodeActionOptions>> codeActionProvider;

        /**
         * The server provides code lens.
         */
        std::optional<CodeLensOptions> codeLensProvider;

        /**
         * The server provides document link support.
         */
        std::optional<DocumentLinkOptions> documentLinkProvider;

        /**
         * The server provides color provider support.
         *
         * @version 3.6.0
         */
        std::optional<std::variant<boolean, DocumentColorOptions, DocumentColorRegistrationOptions>> colorProvider;

        /**
         * The server provides document formatting.
         */
        std::optional<std::variant<boolean, DocumentFormattingOptions>> documentFormattingProvider;

        /**
         * The server provides document range formatting.
         */
        std::optional<std::variant<boolean, DocumentRangeFormattingOptions>> documentRangeFormattingProvider;

        /**
         * The server provides document formatting on typing.
         */
        std::optional<DocumentOnTypeFormattingOptions> documentOnTypeFormattingProvider;

        /**
         * The server provides rename support. RenameOptions may only be
         * specified if the client states that it supports
         * @code prepareSupport@endcode in its initial @code initialize@endcode request.
         */
        std::optional<std::variant<boolean, RenameOptions>> renameProvider;

        /**
         * The server provides folding provider support.
         *
         * @version 3.10.0
         */
        std::optional<std::variant<boolean, FoldingRangeOptions, FoldingRangeRegistrationOptions>> foldingRangeProvider;

        /**
         * The server provides execute command support.
         */
        std::optional<ExecuteCommandOptions> executeCommandProvider;

        /**
         * The server provides selection range support.
         *
         * @version 3.15.0
         */
        std::optional<std::variant<boolean, SelectionRangeOptions, SelectionRangeRegistrationOptions>> selectionRangeProvider;

        /**
         * The server provides linked editing range support.
         *
         * @version 3.16.0
         */
        std::optional<std::variant<boolean, LinkedEditingRangeOptions, LinkedEditingRangeRegistrationOptions>> linkedEditingRangeProvider;

        /**
         * The server provides call hierarchy support.
         *
         * @version 3.16.0
         */
        std::optional<std::variant<boolean, CallHierarchyOptions, CallHierarchyRegistrationOptions>> callHierarchyProvider;

        /**
         * The server provides semantic tokens support.
         *
         * @version 3.16.0
         */
        std::optional<std::variant<SemanticTokensOptions, SemanticTokensRegistrationOptions>> semanticTokensProvider;

        /**
         * Whether server provides moniker support.
         *
         * @version 3.16.0
         */
        std::optional<std::variant<boolean, MonikerOptions, MonikerRegistrationOptions>> monikerProvider;

        /**
         * The server provides type hierarchy support.
         *
         * @version 3.17.0
         */
        std::optional<std::variant<boolean, TypeHierarchyOptions, TypeHierarchyRegistrationOptions>> typeHierarchyProvider;

        /**
         * The server provides inline values.
         *
         * @version 3.17.0
         */
        std::optional<std::variant<boolean, InlineValueOptions, InlineValueRegistrationOptions>> inlineValueProvider;

        /**
         * The server provides inlay hints.
         *
         * @version 3.17.0
         */
        std::optional<std::variant<boolean, InlayHintOptions, InlayHintRegistrationOptions>> inlayHintProvider;

        /**
         * The server has support for pull model diagnostics.
         *
         * @version 3.17.0
         */
        std::optional<std::variant<DiagnosticOptions, DiagnosticRegistrationOptions>> diagnosticProvider;

        /**
         * The server provides workspace symbol support.
         */
        std::optional<std::variant<boolean, WorkspaceSymbolOptions>> workspaceSymbolProvider;

        /**
         * The server provides inline completions.
         *
         * @version 3.18.0
         */
        std::optional<std::variant<boolean, InlineCompletionOptions>> inlineCompletionProvider;

        /**
         * Text document specific server capabilities.
         *
         * @version 3.18.0
         */
        std::optional<_TextDocument> textDocument;

        /**
         * Workspace specific server capabilities
         */
        std::optional<_Workspace> workspace;

        /**
         * Experimental server capabilities.
         */
        std::optional<LSPAny> experimental;
};

struct ServerInfo {
        /**
         * The name of the server as defined by the server.
         */
        string name;

        /**
         * The server's version as defined by the server.
         */
        std::optional<string> version;
};

struct InitializeResult {
        /**
         * The capabilities the language server provides.
         */
        ServerCapabilities capabilities;

        /**
         * Information about the server.
         *
         * @version 3.15.0
         */
        std::optional<ServerInfo> serverInfo;
};

struct InitializedParams {
};

/**
 * General parameters to register for a capability.
 */
struct Registration {
        /**
         * The id used to register the request. The id can be used to deregister
         * the request again.
         */
        string id;

        /**
         * The method / capability to register for.
         */
        string method;

        /**
         * Options necessary for the registration.
         */
        std::optional<LSPAny> registerOptions;
};

struct RegistrationParams {
        std::vector<Registration> registrations;
};

/**
 * General parameters to unregister a capability.
 */
struct Unregistration {
        /**
         * The id used to unregister the request or notification. Usually an id
         * provided during the register request.
         */
        string id;

        /**
         * The method / capability to unregister for.
         */
        string method;
};

struct UnregistrationParams {
        // This should correctly be named @code unregistrations@endcode. However changing this
        // is a breaking change and needs to wait until we deliver a 4.x version
        // of the specification.
        std::vector<Unregistration> unregisterations;
};

struct SetTraceParams {
        /**
         * The new value that should be assigned to the trace setting.
         */
        TraceValue value;
};

struct LogTraceParams {
        /**
         * The message to be logged.
         */
        string message;

        /**
         * Additional information that can be computed if the @code trace@endcode configuration
         * is set to @code 'verbose'@endcode
         */
        std::optional<string> verbose;
};

struct TextDocumentItem {
        /**
         * The text document's URI.
         */
        DocumentUri uri;

        /**
         * The text document's language identifier.
         */
        string languageId;

        /**
         * The version number of this document (it will increase after each
         * change, including undo/redo).
         */
        integer version;

        /**
         * The content of the opened text document.
         */
        string text;
};

struct DidOpenTextDocumentParams {
        /**
         * The document that was opened.
         */
        TextDocumentItem textDocument;
};

/**
 * A literal to identify a text document in the client.
 */
struct TextDocumentIdentifier {
        /**
         * The text document's uri.
         */
        DocumentUri uri;
};

struct VersionedTextDocumentIdentifier : TextDocumentIdentifier {
        /**
         * The version number of this document.
         *
         * The version number of a document will increase after each change,
         * including undo/redo. The number doesn't need to be consecutive.
         */
        integer version;
};

/**
 * Position in a text document expressed as zero-based line and character
 * offset. Prior to 3.17 the offsets were always based on a UTF-16 string
 * representation. So a string of the form @code a𐐀b@endcode the character offset of the
 * character @code a@endcode is 0, the character offset of @code 𐐀@endcode is 1 and the character
 * offset of b is 3 since @code 𐐀@endcode is represented using two code units in UTF-16.
 * Since 3.17 clients and servers can agree on a different string encoding
 * representation (e.g. UTF-8). The client announces it's supported encoding
 * via the client capability [@code general.positionEncodings@endcode](https://microsoft.github.io/language-server-protocol/specifications/specification-current/#clientCapabilities).
 * The value is an array of position encodings the client supports, with
 * decreasing preference (e.g. the encoding at index @code 0@endcode is the most preferred
 * one). To stay backwards compatible the only mandatory encoding is UTF-16
 * represented via the string @code utf-16@endcode. The server can pick one of the
 * encodings offered by the client and signals that encoding back to the
 * client via the initialize result's property
 * [@code capabilities.positionEncoding@endcode](https://microsoft.github.io/language-server-protocol/specifications/specification-current/#serverCapabilities). If the string value
 * @code utf-16@endcode is missing from the client's capability @code general.positionEncodings@endcode
 * servers can safely assume that the client supports UTF-16. If the server
 * omits the position encoding in its initialize result the encoding defaults
 * to the string value @code utf-16@endcode. Implementation considerations: since the
 * conversion from one encoding into another requires the content of the
 * file / line the conversion is best done where the file is read which is
 * usually on the server side.
 *
 * Positions are line end character agnostic. So you can not specify a position
 * that denotes @code \\r|\\n@endcode or @code \\n|@endcode where @code |@endcode represents the character offset.
 *
 * @version 3.17.0 - support for negotiated position encoding.
 */
struct Position {
        /**
         * Line position in a document (zero-based).
         *
         * If a line number is greater than the number of lines in a document, it defaults back to the number of lines in the document.
         * If a line number is negative, it defaults to 0.
         */
        uinteger line;

        /**
         * Character offset on a line in a document (zero-based).
         *
         * The meaning of this offset is determined by the negotiated
         * @code PositionEncodingKind@endcode.
         *
         * If the character value is greater than the line length it defaults back to the
         * line length.
         */
        uinteger character;
};

/**
 * A range in a text document expressed as (zero-based) start and end positions.
 *
 * If you want to specify a range that contains a line including the line ending
 * character(s) then use an end position denoting the start of the next line.
 * For example:
 * @code
 * {
 *     start: { line: 5, character: 23 }
 *     end : { line 6, character : 0 }
 * }
 * @endcode
 */
struct Range {
        /**
         * The range's start position.
         */
        Position start;
        /**
         * The range's end position.
         */
        Position end;
};

/**
 * An event describing a change to a text document. If only a text is provided
 * it is considered to be the full content of the document.
 */
struct TextDocumentContentChangeEvent {
        struct _Text {
                string text;
        };

        /**
         * The range of the document that changed.
         */
        Range range;

        /**
         * The optional length of the range that got replaced.
         *
         * @deprecated use range instead.
         */
        std::optional<uinteger> rangeLength;

        /**
         * The new text for the provided range.
         */
        std::variant<string, _Text> text;
};

struct DidChangeTextDocumentParams {
        /**
         * The document that did change. The version number points
         * to the version after all provided content changes have
         * been applied.
         */
        VersionedTextDocumentIdentifier textDocument;

        /**
         * The actual content changes. The content changes describe single state
         * changes to the document. So if there are two content changes c1 (at
         * array index 0) and c2 (at array index 1) for a document in state S then
         * c1 moves the document from S to S' and c2 from S' to S''. So c1 is
         * computed on the state S and c2 is computed on the state S'.
         *
         * To mirror the content of a document using change events use the following
         * approach:
         * - start with the same initial content
         * - apply the 'textDocument/didChange' notifications in the order you
         *   receive them.
         * - apply the @code TextDocumentContentChangeEvent@endcode s in a single notification
         *   in the order you receive them.
         */
        std::vector<TextDocumentContentChangeEvent> contentChanges;
};

/**
 * Represents reasons why a text document is saved.
 */
enum class TextDocumentSaveReason {
        /**
         * Manually triggered, e.g. by the user pressing save, by starting
         * debugging, or by an API call.
         */
        Manual = 1,

        /**
         * Automatic after a delay.
         */
        AfterDelay = 2,

        /**
         * When the editor lost focus.
         */
        FocusOut = 3

};

/**
 * The parameters send in a will save text document notification.
 */
struct WillSaveTextDocumentParams {
        /**
         * The document that will be saved.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The 'TextDocumentSaveReason'.
         */
        TextDocumentSaveReason reason;
};

struct TextEdit {
        /**
         * The range of the text document to be manipulated. To insert
         * text into a document create a range where start === end.
         */
        Range range;

        /**
         * The string to be inserted. For delete operations use an
         * empty string.
         */
        string newText;
};

struct DidSaveTextDocumentParams {
        /**
         * The document that was saved.
         */
        TextDocumentIdentifier textDocument;

        /**
         * Optional the content when saved. Depends on the includeText value
         * when the save notification was requested.
         */
        std::optional<string> text;
};

struct DidCloseTextDocumentParams {
        /**
         * The document that was closed.
         */
        TextDocumentIdentifier textDocument;
};

/**
 * A notebook cell kind.
 *
 * @version 3.17.0
 */
enum class NotebookCellKind {
        /**
         * A markup-cell is formatted source that is used for display.
         */
        Markup = 1,

        /**
         * A code-cell is source code.
         */
        Code = 2

};

struct ExecutionSummary {
        /**
         * A strict monotonically increasing value
         * indicating the execution order of a cell
         * inside a notebook.
         */
        uinteger executionOrder;

        /**
         * Whether the execution was successful or
         * not if known by the client.
         */
        std::optional<boolean> success;
};

/**
 * A notebook cell.
 *
 * A cell's document URI must be unique across ALL notebook
 * cells and can therefore be used to uniquely identify a
 * notebook cell or the cell's text document.
 *
 * @version 3.17.0
 */
struct NotebookCell {
        /**
         * The cell's kind
         */
        NotebookCellKind kind;

        /**
         * The URI of the cell's text document
         * content.
         */
        DocumentUri document;

        /**
         * Additional metadata stored with the cell.
         */
        std::optional<LSPObject> metadata;

        /**
         * Additional execution summary information
         * if supported by the client.
         */
        ExecutionSummary executionSummary;
};

/**
 * A notebook document.
 *
 * @version 3.17.0
 */
struct NotebookDocument {
        /**
         * The notebook document's URI.
         */
        URI uri;

        /**
         * The type of the notebook.
         */
        string notebookType;

        /**
         * The version number of this document (it will increase after each
         * change, including undo/redo).
         */
        integer version;

        /**
         * Additional metadata stored with the notebook
         * document.
         */
        std::optional<LSPObject> metadata;

        /**
         * The cells of a notebook.
         */
        std::vector<NotebookCell> cells;
};

/**
 * The params sent in an open notebook document notification.
 *
 * @version 3.17.0
 */
struct DidOpenNotebookDocumentParams {
        /**
         * The notebook document that got opened.
         */
        NotebookDocument notebookDocument;

        /**
         * The text documents that represent the content
         * of a notebook cell.
         */
        std::vector<TextDocumentItem> cellTextDocuments;
};

/**
 * A versioned notebook document identifier.
 *
 * @version 3.17.0
 */
struct VersionedNotebookDocumentIdentifier {
        /**
         * The version number of this notebook document.
         */
        integer version;

        /**
         * The notebook document's URI.
         */
        URI uri;
};

/**
 * A change describing how to move a @code NotebookCell@endcode
 * array from state S to S'.
 *
 * @version 3.17.0
 */
struct NotebookCellArrayChange {
        /**
         * The start offset of the cell that changed.
         */
        uinteger start;

        /**
         * The deleted cells
         */
        uinteger deleteCount;

        /**
         * The new cells, if any
         */
        std::optional<std::vector<NotebookCell>> cells;
};

/**
 * A change event for a notebook document.
 *
 * @version 3.17.0
 */
struct NotebookDocumentChangeEvent {
        struct _Cells {
                struct _Structure {
                        /**
                         * The change to the cell array.
                         */
                        NotebookCellArrayChange array;

                        /**
                         * Additional opened cell text documents.
                         */
                        std::optional<std::vector<TextDocumentItem>> didOpen;

                        /**
                         * Additional closed cell text documents.
                         */
                        std::optional<std::vector<TextDocumentIdentifier>> didClose;
                };

                struct _TextContent {
                        VersionedTextDocumentIdentifier document;
                        std::vector<TextDocumentContentChangeEvent> changes;
                };

                /**
                 * Changes to the cell structure to add or
                 * remove cells.
                 */
                std::optional<_Structure> structure;

                /**
                 * Changes to notebook cells properties like its
                 * kind, execution summary or metadata.
                 */
                std::optional<std::vector<NotebookCell>> data;

                /**
                 * Changes to the text content of notebook cells.
                 */
                std::optional<std::vector<_TextContent>> textContent;
        };

        /**
         * The changed meta data if any.
         */
        std::optional<LSPObject> metadata;

        /**
         * Changes to cells
         */
        std::optional<_Cells> cells;
};

/**
 * The params sent in a change notebook document notification.
 *
 * @version 3.17.0
 */
struct DidChangeNotebookDocumentParams {
        /**
         * The notebook document that did change. The version number points
         * to the version after all provided changes have been applied.
         */
        VersionedNotebookDocumentIdentifier notebookDocument;

        /**
         * The actual changes to the notebook document.
         *
         * The change describes single state change to the notebook document.
         * So it moves a notebook document, its cells and its cell text document
         * contents from state S to S'.
         *
         * To mirror the content of a notebook using change events use the
         * following approach:
         * - start with the same initial content
         * - apply the 'notebookDocument/didChange' notifications in the order
         *   you receive them.
         */
        NotebookDocumentChangeEvent change;
};

/**
 * A literal to identify a notebook document in the client.
 *
 * @version 3.17.0
 */
struct NotebookDocumentIdentifier {
        /**
         * The notebook document's URI.
         */
        URI uri;
};

/**
 * The params sent in a save notebook document notification.
 *
 * @version 3.17.0
 */
struct DidSaveNotebookDocumentParams {
        /**
         * The notebook document that got saved.
         */
        NotebookDocumentIdentifier notebookDocument;
};

/**
 * The params sent in a close notebook document notification.
 *
 * @version 3.17.0
 */
struct DidCloseNotebookDocumentParams {
        /**
         * The notebook document that got closed.
         */
        NotebookDocumentIdentifier notebookDocument;

        /**
         * The text documents that represent the content
         * of a notebook cell that got closed.
         */
        std::vector<TextDocumentIdentifier> cellTextDocuments;
};

struct TextDocumentPositionParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The position inside the text document.
         */
        Position position;
};

struct PartialResultParams {
        /**
         * An optional token that a server can use to report partial results (e.g. streaming) to
         * the client.
         */
        std::optional<ProgressToken> partialResultToken;
};

struct DeclarationParams : TextDocumentPositionParams, WorkDoneProgressParams, PartialResultParams {
};

/**
 * Represents a location inside a resource, such as a line
 * inside a text file.
 */
struct Location {
        DocumentUri uri;
        Range range;
};

/**
 * Represents the connection of two locations. Provides additional metadata over normal {@link Location locations@endlink},
 * including an origin range.
 */
struct LocationLink {
        /**
         * Span of the origin of this link.
         *
         * Used as the underlined span for mouse interaction. Defaults to the word
         * range at the mouse position.
         */
        std::optional<Range> originSelectionRange;

        /**
         * The target resource identifier of this link.
         */
        DocumentUri targetUri;

        /**
         * The full target range of this link. If the target for example is a symbol
         * then target range is the range enclosing this symbol not including
         * leading/trailing whitespace but everything else like comments. This
         * information is typically used to highlight the range in the editor.
         */
        Range targetRange;

        /**
         * The range that should be selected and revealed when this link is being
         * followed, e.g the name of a function. Must be contained by the
         * @code targetRange@endcode. See also @code DocumentSymbol#range@endcode
         */
        Range targetSelectionRange;
};

struct DefinitionParams : TextDocumentPositionParams, WorkDoneProgressParams, PartialResultParams {
};

struct TypeDefinitionParams : TextDocumentPositionParams, WorkDoneProgressParams, PartialResultParams {
};

struct ImplementationParams : TextDocumentPositionParams, WorkDoneProgressParams, PartialResultParams {
};

struct ReferenceContext {
        /**
         * Include the declaration of the current symbol.
         */
        boolean includeDeclaration;
};

struct ReferenceParams : TextDocumentPositionParams, WorkDoneProgressParams, PartialResultParams {
        ReferenceContext context;
};

struct CallHierarchyPrepareParams : TextDocumentPositionParams, WorkDoneProgressParams {
};

struct CallHierarchyItem {
        /**
         * The name of this item.
         */
        string name;

        /**
         * The kind of this item.
         */
        SymbolKind kind;

        /**
         * Tags for this item.
         */
        std::optional<std::vector<SymbolTag>> tags;

        /**
         * More detail for this item, e.g. the signature of a function.
         */
        std::optional<string> detail;

        /**
         * The resource identifier of this item.
         */
        DocumentUri uri;

        /**
         * The range enclosing this symbol not including leading/trailing whitespace
         * but everything else, e.g. comments and code.
         */
        Range range;

        /**
         * The range that should be selected and revealed when this symbol is being
         * picked, e.g. the name of a function. Must be contained by the
         * @link CallHierarchyItem::range range@endlink.
         */
        Range selectionRange;

        /**
         * A data entry field that is preserved between a call hierarchy prepare and
         * incoming calls or outgoing calls requests.
         */
        std::optional<LSPAny> data;
};

struct CallHierarchyIncomingCallsParams : WorkDoneProgressParams, PartialResultParams {
        CallHierarchyItem item;
};

struct CallHierarchyIncomingCall {
        /**
         * The item that makes the call.
         */
        CallHierarchyItem from;

        /**
         * The ranges at which the calls appear. This is relative to the caller
         * denoted by @link CallHierarchyIncomingCall::from from@endlink.
         */
        std::vector<Range> fromRanges;
};

struct CallHierarchyOutgoingCallsParams : WorkDoneProgressParams, PartialResultParams {
        CallHierarchyItem item;
};

struct CallHierarchyOutgoingCall {
        /**
         * The item that is called.
         */
        CallHierarchyItem to;

        /**
         * The range at which this item is called. This is the range relative to
         * the caller, e.g the item passed to @code callHierarchy/outgoingCalls@endcode request.
         */
        std::vector<Range> fromRanges;
};

struct TypeHierarchyPrepareParams : TextDocumentPositionParams, WorkDoneProgressParams {
};

struct TypeHierarchyItem {
        /**
         * The name of this item.
         */
        string name;

        /**
         * The kind of this item.
         */
        SymbolKind kind;

        /**
         * Tags for this item.
         */
        std::optional<std::vector<SymbolTag>> tags;

        /**
         * More detail for this item, e.g. the signature of a function.
         */
        std::optional<string> detail;

        /**
         * The resource identifier of this item.
         */
        DocumentUri uri;

        /**
         * The range enclosing this symbol not including leading/trailing whitespace
         * but everything else, e.g. comments and code.
         */
        Range range;

        /**
         * The range that should be selected and revealed when this symbol is being
         * picked, e.g. the name of a function. Must be contained by the
         * [@code range@endcode](#TypeHierarchyItem.range).
         */
        Range selectionRange;

        /**
         * A data entry field that is preserved between a type hierarchy prepare and
         * supertypes or subtypes requests. It could also be used to identify the
         * type hierarchy in the server, helping improve the performance on
         * resolving supertypes and subtypes.
         */
        std::optional<LSPAny> data;
};

struct TypeHierarchySupertypesParams : WorkDoneProgressParams, PartialResultParams {
        TypeHierarchyItem item;
};

struct TypeHierarchySubtypesParams : WorkDoneProgressParams, PartialResultParams {
        TypeHierarchyItem item;
};

struct DocumentHighlightParams : TextDocumentPositionParams, WorkDoneProgressParams, PartialResultParams {
};

/**
 * A document highlight kind.
 */
enum class DocumentHighlightKind {
        /**
         * A textual occurrence.
         */
        Text = 1,

        /**
         * Read-access of a symbol, like reading a variable.
         */
        Read = 2,

        /**
         * Write-access of a symbol, like writing to a variable.
         */
        Write = 3

};

/**
 * A document highlight is a range inside a text document which deserves
 * special attention. Usually a document highlight is visualized by changing
 * the background color of its range.
 *
 */
struct DocumentHighlight {
        /**
         * The range this highlight applies to.
         */
        Range range;

        /**
         * The highlight kind, default is DocumentHighlightKind.Text.
         */
        std::optional<DocumentHighlightKind> kind;
};

struct DocumentLinkParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The document to provide document links for.
         */
        TextDocumentIdentifier textDocument;
};

/**
 * A document link is a range in a text document that links to an internal or
 * external resource, like another text document or a web site.
 */
struct DocumentLink {
        /**
         * The range this link applies to.
         */
        Range range;

        /**
         * The uri this link points to. If missing a resolve request is sent later.
         */
        std::optional<URI> target;

        /**
         * The tooltip text when you hover over this link.
         *
         * If a tooltip is provided, is will be displayed in a string that includes
         * instructions on how to trigger the link, such as @code (ctrl + click)@endcode.
         * The specific instructions vary depending on OS, user settings, and
         * localization.
         *
         * @version 3.15.0
         */
        std::optional<string> tooltip;

        /**
         * A data entry field that is preserved on a document link between a
         * DocumentLinkRequest and a DocumentLinkResolveRequest.
         */
        std::optional<LSPAny> data;
};

struct HoverParams : TextDocumentPositionParams, WorkDoneProgressParams {
};

/**
 * A @code MarkupContent@endcode literal represents a string value which content is
 * interpreted base on its kind flag. Currently the protocol supports
 * @code plaintext@endcode and @code markdown@endcode as markup kinds.
 *
 * If the kind is @code markdown@endcode then the value can contain fenced code blocks like
 * in GitHub issues.
 *
 * Here is an example how such a string can be constructed using
 * JavaScript / TypeScript:
 * @code
 * let markdown: MarkdownContent = {
 *         kind: MarkupKind.Markdown,
 *         value: [
 *                 '# Header',
 *                 'Some text',
 *                 '```typescript',
 *                 'someCode();',
 *                 '```'
 *         ].join('\n')
 * };
 * @endcode
 *
 * *Please Note* that clients might sanitize the return markdown. A client could
 * decide to remove HTML from the markdown to avoid script execution.
 */
struct MarkupContent {
        /**
         * The type of the Markup
         */
        MarkupKind kind;

        /**
         * The content itself
         */
        string value;
};

struct _MarkedString {
        string language;
        string value;
};

/**
 * MarkedString can be used to render human readable text. It is either a
 * markdown string or a code-block that provides a language and a code snippet.
 * The language identifier is semantically equal to the optional language
 * identifier in fenced code blocks in GitHub issues.
 *
 * The pair of a language and a value is an equivalent to markdown:
 * ```${language}
 * ${value}
 * ```
 *
 * Note that markdown strings will be sanitized - that means html will be
 * escaped.
 *
 * @deprecated use MarkupContent instead.
 */
using MarkedString = std::variant<string, _MarkedString>;

/**
 * The result of a hover request.
 */
struct Hover {
        /**
         * The hover's content
         */
        std::variant<MarkedString, std::vector<MarkedString>, MarkupContent> contents;

        /**
         * An optional range is a range inside a text document
         * that is used to visualize a hover, e.g. by changing the background color.
         */
        std::optional<Range> range;
};

struct CodeLensParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The document to request code lens for.
         */
        TextDocumentIdentifier textDocument;
};

/**
 * A code lens represents a command that should be shown along with
 * source text, like the number of references, a way to run tests, etc.
 *
 * A code lens is unresolved when no command is associated to it. For
 * performance reasons the creation of a code lens and resolving should be done
 * in two stages.
 */
struct CodeLens {
        /**
         * The range in which this code lens is valid. Should only span a single
         * line.
         */
        Range range;

        /**
         * The command this code lens represents.
         */
        std::optional<Command> command;

        /**
         * A data entry field that is preserved on a code lens item between
         * a code lens and a code lens resolve request.
         */
        std::optional<LSPAny> data;
};

struct FoldingRangeParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;
};

/**
 * Represents a folding range. To be valid, start and end line must be bigger
 * than zero and smaller than the number of lines in the document. Clients
 * are free to ignore invalid ranges.
 */
struct FoldingRange {
        /**
         * The zero-based start line of the range to fold. The folded area starts
         * after the line's last character. To be valid, the end must be zero or
         * larger and smaller than the number of lines in the document.
         */
        uinteger startLine;

        /**
         * The zero-based character offset from where the folded range starts. If
         * not defined, defaults to the length of the start line.
         */
        uinteger startCharacter;

        /**
         * The zero-based end line of the range to fold. The folded area ends with
         * the line's last character. To be valid, the end must be zero or larger
         * and smaller than the number of lines in the document.
         */
        uinteger endLine;

        /**
         * The zero-based character offset before the folded range ends. If not
         * defined, defaults to the length of the end line.
         */
        uinteger endCharacter;

        /**
         * Describes the kind of the folding range such as @code comment@endcode or @code region@endcode.
         * The kind is used to categorize folding ranges and used by commands like
         * 'Fold all comments'. See [FoldingRangeKind](#FoldingRangeKind) for an
         * enumeration of standardized kinds.
         */
        FoldingRangeKind kind;

        /**
         * The text that the client should show when the specified range is
         * collapsed. If not defined or not supported by the client, a default
         * will be chosen by the client.
         *
         * @version 3.17.0 - proposed
         */
        std::optional<string> collapsedText;
};

struct SelectionRangeParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The positions inside the text document.
         */
        std::vector<Position> positions;
};

struct SelectionRange {
        /**
         * The @link Range range@endlink of this selection range.
         */
        Range range;

        /**
         * The parent selection range containing this range. Therefore
         * @code parent.range@endcode must contain @code this.range@endcode.
         */
        std::optional<std::unique_ptr<SelectionRange>> parent;
};

struct DocumentSymbolParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;
};

/**
 * Represents programming constructs like variables, classes, interfaces etc.
 * that appear in a document. Document symbols can be hierarchical and they
 * have two ranges: one that encloses its definition and one that points to its
 * most interesting range, e.g. the range of an identifier.
 */
struct DocumentSymbol {
        /**
         * The name of this symbol. Will be displayed in the user interface and
         * therefore must not be an empty string or a string only consisting of
         * white spaces.
         */
        string name;

        /**
         * More detail for this symbol, e.g the signature of a function.
         */
        std::optional<string> detail;

        /**
         * The kind of this symbol.
         */
        SymbolKind kind;

        /**
         * Tags for this document symbol.
         *
         * @version 3.16.0
         */
        std::optional<std::vector<SymbolTag>> tags;

        /**
         * Indicates if this symbol is deprecated.
         *
         * @deprecated Use tags instead
         */
        std::optional<boolean> deprecated;

        /**
         * The range enclosing this symbol not including leading/trailing whitespace
         * but everything else like comments. This information is typically used to
         * determine if the clients cursor is inside the symbol to reveal it in the
         * UI.
         */
        Range range;

        /**
         * The range that should be selected and revealed when this symbol is being
         * picked, e.g. the name of a function. Must be contained by the @code range@endcode.
         */
        Range selectionRange;

        /**
         * Children of this symbol, e.g. properties of a class.
         */
        std::optional<std::vector<DocumentSymbol>> children;
};

/**
 * Represents information about programming constructs like variables, classes,
 * interfaces etc.
 *
 * @deprecated use DocumentSymbol or WorkspaceSymbol instead.
 */
struct SymbolInformation {
        /**
         * The name of this symbol.
         */
        string name;

        /**
         * The kind of this symbol.
         */
        SymbolKind kind;

        /**
         * Tags for this symbol.
         *
         * @version 3.16.0
         */
        std::optional<std::vector<SymbolTag>> tags;

        /**
         * Indicates if this symbol is deprecated.
         *
         * @deprecated Use tags instead
         */
        std::optional<boolean> deprecated;

        /**
         * The location of this symbol. The location's range is used by a tool
         * to reveal the location in the editor. If the symbol is selected in the
         * tool the range's start information is used to position the cursor. So
         * the range usually spans more then the actual symbol's name and does
         * normally include things like visibility modifiers.
         *
         * The range doesn't have to denote a node range in the sense of an abstract
         * syntax tree. It can therefore not be used to re-construct a hierarchy of
         * the symbols.
         */
        Location location;

        /**
         * The name of the symbol containing this symbol. This information is for
         * user interface purposes (e.g. to render a qualifier in the user interface
         * if necessary). It can't be used to re-infer a hierarchy for the document
         * symbols.
         */
        std::optional<string> containerName;
};

struct SemanticTokensParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;
};

struct SemanticTokens {
        /**
         * An optional result id. If provided and clients support delta updating
         * the client will include the result id in the next semantic token request.
         * A server can then instead of computing all semantic tokens again simply
         * send a delta.
         */
        std::optional<string> resultId;

        /**
         * The actual tokens.
         */
        std::vector<uinteger> data;
};

struct SemanticTokensPartialResult {
        std::vector<uinteger> data;
};

struct SemanticTokensDeltaParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The result id of a previous response. The result Id can either point to
         * a full response or a delta response depending on what was received last.
         */
        string previousResultId;
};

struct SemanticTokensEdit {
        /**
         * The start offset of the edit.
         */
        uinteger start;

        /**
         * The count of elements to remove.
         */
        uinteger deleteCount;

        /**
         * The elements to insert.
         */
        std::vector<uinteger> data;
};

struct SemanticTokensDelta {
        std::optional<string> resultId;

        /**
         * The semantic token edits to transform a previous result into a new
         * result.
         */
        std::vector<SemanticTokensEdit> edits;
};

struct SemanticTokensDeltaPartialResult {
        std::vector<SemanticTokensEdit> edits;
};

struct SemanticTokensRangeParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The range the semantic tokens are requested for.
         */
        Range range;
};

/**
 * A parameter literal used in inlay hint requests.
 *
 * @version 3.17.0
 */
struct InlayHintParams : WorkDoneProgressParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The visible document range for which inlay hints should be computed.
         */
        Range range;
};

/**
 * An inlay hint label part allows for interactive and composite labels
 * of inlay hints.
 *
 * @version 3.17.0
 */
struct InlayHintLabelPart {
        /**
         * The value of this label part.
         */
        string value;

        /**
         * The tooltip text when you hover over this label part. Depending on
         * the client capability @code inlayHint.resolveSupport@endcode clients
         * might resolve this property late using the resolve request.
         */
        std::optional<std::variant<string, MarkupContent>> tooltip;

        /**
         * An optional source code location that represents this
         * label part.
         *
         * The editor will use this location for the hover and for code navigation
         * features: This part will become a clickable link that resolves to the
         * definition of the symbol at the given location (not necessarily the
         * location itself), it shows the hover that shows at the given location,
         * and it shows a context menu with further code navigation commands.
         *
         * Depending on the client capability @code inlayHint.resolveSupport@endcode clients
         * might resolve this property late using the resolve request.
         */
        std::optional<Location> location;

        /**
         * An optional command for this label part.
         *
         * Depending on the client capability @code inlayHint.resolveSupport@endcode clients
         * might resolve this property late using the resolve request.
         */
        std::optional<Command> command;
};

/**
 * Inlay hint kinds.
 *
 * @version 3.17.0
 */
enum class InlayHintKind {
        /**
         * An inlay hint that for a type annotation.
         */
        Type = 1,

        /**
         * An inlay hint that is for a parameter.
         */
        Parameter = 2

};

/**
 * Inlay hint information.
 *
 * @version 3.17.0
 */
struct InlayHint {
        /**
         * The position of this hint.
         *
         * If multiple hints have the same position, they will be shown in the order
         * they appear in the response.
         */
        Position position;

        /**
         * The label of this hint. A human readable string or an array of
         * InlayHintLabelPart label parts.
         *
         * *Note* that neither the string nor the label part can be empty.
         */
        std::variant<string, std::vector<InlayHintLabelPart>> label;

        /**
         * The kind of this hint. Can be omitted in which case the client
         * should fall back to a reasonable default.
         */
        std::optional<InlayHintKind> kind;

        /**
         * Optional text edits that are performed when accepting this inlay hint.
         *
         * *Note* that edits are expected to change the document so that the inlay
         * hint (or its nearest variant) is now part of the document and the inlay
         * hint itself is now obsolete.
         *
         * Depending on the client capability @code inlayHint.resolveSupport@endcode clients
         * might resolve this property late using the resolve request.
         */
        std::optional<std::vector<TextEdit>> textEdits;

        /**
         * The tooltip text when you hover over this item.
         *
         * Depending on the client capability @code inlayHint.resolveSupport@endcode clients
         * might resolve this property late using the resolve request.
         */
        std::optional<std::variant<string, MarkupContent>> tooltip;

        /**
         * Render padding before the hint.
         *
         * Note: Padding should use the editor's background color, not the
         * background color of the hint itself. That means padding can be used
         * to visually align/separate an inlay hint.
         */
        std::optional<boolean> paddingLeft;

        /**
         * Render padding after the hint.
         *
         * Note: Padding should use the editor's background color, not the
         * background color of the hint itself. That means padding can be used
         * to visually align/separate an inlay hint.
         */
        std::optional<boolean> paddingRight;

        /**
         * A data entry field that is preserved on an inlay hint between
         * a @code textDocument/inlayHint@endcode and a @code inlayHint/resolve@endcode request.
         */
        std::optional<LSPAny> data;
};

/**
 * @version 3.17.0
 */
struct InlineValueContext {
        /**
         * The stack frame (as a DAP Id) where the execution has stopped.
         */
        uinteger frameId;

        /**
         * The document range where execution has stopped.
         * Typically the end position of the range denotes the line where the
         * inline values are shown.
         */
        Range stoppedLocation;
};

/**
 * A parameter literal used in inline value requests.
 *
 * @version 3.17.0
 */
struct InlineValueParams : WorkDoneProgressParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The document range for which inline values should be computed.
         */
        Range range;

        /**
         * Additional information about the context in which inline values were
         * requested.
         */
        InlineValueContext context;
};

/**
 * Provide inline value as text.
 *
 * @version 3.17.0
 */
struct InlineValueText {
        /**
         * The document range for which the inline value applies.
         */
        Range range;

        /**
         * The text of the inline value.
         */
        string text;
};

/**
 * Provide inline value through a variable lookup.
 *
 * If only a range is specified, the variable name will be extracted from
 * the underlying document.
 *
 * An optional variable name can be used to override the extracted name.
 *
 * @version 3.17.0
 */
struct InlineValueVariableLookup {
        /**
         * The document range for which the inline value applies.
         * The range is used to extract the variable name from the underlying
         * document.
         */
        Range range;

        /**
         * If specified the name of the variable to look up.
         */
        std::optional<string> variableName;

        /**
         * How to perform the lookup.
         */
        boolean caseSensitiveLookup;
};

/**
 * Provide an inline value through an expression evaluation.
 *
 * If only a range is specified, the expression will be extracted from the
 * underlying document.
 *
 * An optional expression can be used to override the extracted expression.
 *
 * @version 3.17.0
 */
struct InlineValueEvaluatableExpression {
        /**
         * The document range for which the inline value applies.
         * The range is used to extract the evaluatable expression from the
         * underlying document.
         */
        Range range;

        /**
         * If specified the expression overrides the extracted expression.
         */
        std::optional<string> expression;
};

/**
 * Inline value information can be provided by different means:
 * - directly as a text value (struct InlineValueText).
 * - as a name to use for a variable lookup (struct InlineValueVariableLookup)
 * - as an evaluatable expression (struct InlineValueEvaluatableExpression)
 * The InlineValue types combines all inline value types into one type.
 *
 * @version 3.17.0
 */
using InlineValue = std::variant<InlineValueText, InlineValueVariableLookup, InlineValueEvaluatableExpression>;

struct MonikerParams : TextDocumentPositionParams, WorkDoneProgressParams, PartialResultParams {
};

/**
 * Moniker uniqueness level to define scope of the moniker.
 */
STRING_ENUM(UniquenessLevel,
        Document, "document",
        Project, "project",
        Group, "group",
        Scheme, "scheme",
        Global, "global"
)

/**
 * The moniker kind.
 */
STRING_ENUM(MonikerKind,
        Import, "import",
        Export, "export",
        Group, "local"
)

/**
 * Moniker definition to match LSIF 0.5 moniker definition.
 */
struct Moniker {
        /**
         * The scheme of the moniker. For example tsc or .Net
         */
        string scheme;

        /**
         * The identifier of the moniker. The value is opaque in LSIF however
         * schema owners are allowed to define the structure if they want.
         */
        string identifier;

        /**
         * The scope in which the moniker is unique
         */
        UniquenessLevel unique;

        /**
         * The moniker kind if known.
         */
        std::optional<MonikerKind> kind;
};

/**
 * How a completion was triggered
 */
enum class CompletionTriggerKind {
        /**
         * Completion was triggered by typing an identifier (24x7 code
         * complete), manual invocation (e.g Ctrl+Space) or via API.
         */
        Invoked = 1,

        /**
         * Completion was triggered by a trigger character specified by
         * the @code triggerCharacters@endcode properties of the
         * @code CompletionRegistrationOptions@endcode.
         */
        TriggerCharacter = 2,

        /**
         * Completion was re-triggered as the current completion list is incomplete.
         */
        TriggerForIncompleteCompletions = 3

};

/**
 * Contains additional information about the context in which a completion
 * request is triggered.
 */
struct CompletionContext {
        /**
         * How the completion was triggered.
         */
        CompletionTriggerKind triggerKind;

        /**
         * The trigger character (a single character) that has trigger code
         * complete. Is undefined if
         * @code triggerKind !== CompletionTriggerKind.TriggerCharacter@endcode
         */
        std::optional<string> triggerCharacter;
};

struct CompletionParams : TextDocumentPositionParams, WorkDoneProgressParams, PartialResultParams {
        /**
         * The completion context. This is only available if the client specifies
         * to send this using the client capability
         * @code completion.contextSupport === true@endcode
         */
        std::optional<CompletionContext> context;
};

/**
 * Additional details for a completion item label.
 *
 * @version 3.17.0
 */
struct CompletionItemLabelDetails {
        /**
         * An optional string which is rendered less prominently directly after
         * {@link CompletionItem::label label @endlink}, without any spacing. Should be
         * used for function signatures or type annotations.
         */
        std::optional<string> detail;

        /**
         * An optional string which is rendered less prominently after
         * {@link CompletionItemLabelDetails::detail @endlink}. Should be used
         * for fully qualified names or file path.
         */
        std::optional<string> description;
};

/**
 * Defines whether the insert text in a completion item should be interpreted as
 * plain text or a snippet.
 */
enum class InsertTextFormat {
        /**
         * The primary text to be inserted is treated as a plain string.
         */
        PlainText = 1,

        /**
         * The primary text to be inserted is treated as a snippet.
         *
         * A snippet can define tab stops and placeholders with @code $1@endcode,
         * @code $2@endcode and @code ${3:foo}@endcode. @code $0@endcode defines
         * the final tab stop, it defaults to the end of the snippet. Placeholders
         * with equal identifiers are linked, that is typing in one will update
         * others too.
         */
        Snippet = 2

};

/**
 * A special text edit to provide an insert and a replace operation.
 *
 * @version 3.16.0
 */
struct InsertReplaceEdit {
        /**
         * The string to be inserted.
         */
        string newText;

        /**
         * The range if the insert is requested
         */
        Range insert;

        /**
         * The range if the replace is requested.
         */
        Range replace;
};

struct CompletionItem {
        /**
         * The label of this completion item.
         *
         * The label property is also by default the text that
         * is inserted when selecting this completion.
         *
         * If label details are provided the label itself should
         * be an unqualified name of the completion item.
         */
        string label;

        /**
         * Additional details for the label
         *
         * @version 3.17.0
         */
        std::optional<CompletionItemLabelDetails> labelDetails;

        /**
         * The kind of this completion item. Based of the kind
         * an icon is chosen by the editor. The standardized set
         * of available values is defined in @code CompletionItemKind@endcode.
         */
        std::optional<CompletionItemKind> kind;

        /**
         * Tags for this completion item.
         *
         * @version 3.15.0
         */
        std::optional<std::vector<CompletionItemTag>> tags;

        /**
         * A human-readable string with additional information
         * about this item, like type or symbol information.
         */
        std::optional<string> detail;

        /**
         * A human-readable string that represents a doc-comment.
         */
        std::optional<std::variant<string, MarkupContent>> documentation;

        /**
         * Indicates if this item is deprecated.
         *
         * @deprecated Use @code tags@endcode instead if supported.
         */
        std::optional<boolean> deprecated;

        /**
         * Select this item when showing.
         *
         * *Note* that only one completion item can be selected and that the
         * tool / client decides which item that is. The rule is that the *first*
         * item of those that match best is selected.
         */
        std::optional<boolean> preselect;

        /**
         * A string that should be used when comparing this item
         * with other items. When omitted the label is used
         * as the sort text for this item.
         */
        std::optional<string> sortText;

        /**
         * A string that should be used when filtering a set of
         * completion items. When omitted the label is used as the
         * filter text for this item.
         */
        std::optional<string> filterText;

        /**
         * A string that should be inserted into a document when selecting
         * this completion. When omitted the label is used as the insert text
         * for this item.
         *
         * The @code insertText@endcode is subject to interpretation by the client
         * side. Some tools might not take the string literally. For example
         * VS Code when code complete is requested in this example
         * @code con<cursor position>@endcode and a completion item with an
         * @code insertText@endcode of @code console@endcode is provided it will
         * only insert @code sole@endcode. Therefore it is recommended to use
         * @code textEdit@endcode instead since it avoids additional client
         * side interpretation.
         */
        std::optional<string> insertText;

        /**
         * The format of the insert text. The format applies to both the
         * @code insertText@endcode property and the @code newText@endcode property
         * of a provided @code textEdit@endcode. If omitted defaults to
         * @code InsertTextFormat.PlainText@endcode.
         *
         * Please note that the insertTextFormat doesn't apply to
         * @code additionalTextEdits@endcode.
         */
        std::optional<InsertTextFormat> insertTextFormat;

        /**
         * How whitespace and indentation is handled during completion
         * item insertion. If not provided the client's default value depends on
         * the @code textDocument.completion.insertTextMode@endcode client capability.
         *
         * @version 3.16.0
         * @version 3.17.0 - support for @code textDocument.completion.insertTextMode@endcode
         */
        std::optional<InsertTextMode> insertTextMode;

        /**
         * An edit which is applied to a document when selecting this completion.
         * When an edit is provided the value of @code insertText@endcode is ignored.
         *
         * @note The range of the edit must be a single line range and it must
         * contain the position at which completion has been requested.
         *
         * Most editors support two different operations when accepting a completion
         * item. One is to insert a completion text and the other is to replace an
         * existing text with a completion text. Since this can usually not be
         * predetermined by a server it can report both ranges. Clients need to
         * signal support for @code InsertReplaceEdit@endcode s via the
         * @code textDocument.completion.completionItem.insertReplaceSupport@endcode
         * client capability property.
         *
         * @note The text edit's range as well as both ranges from an insert
         * replace edit must be a [single line] and they must contain the position
         * at which completion has been requested.
         *
         * @note If an @code InsertReplaceEdit@endcode is returned the edit's
         * insert range must be a prefix of the edit's replace range, that means
         * it must be contained and starting at the same position.
         *
         * @version 3.16.0 additional type @code InsertReplaceEdit@endcode
         */
        std::optional<std::variant<TextEdit, InsertReplaceEdit>> textEdit;

        /**
         * The edit text used if the completion item is part of a CompletionList and
         * CompletionList defines an item default for the text edit range.
         *
         * Clients will only honor this property if they opt into completion list
         * item defaults using the capability @code completionList.itemDefaults@endcode.
         *
         * If not provided and a list's default range is provided the label
         * property is used as a text.
         *
         * @version 3.17.0
         */
        std::optional<string> textEditText;

        /**
         * An optional array of additional text edits that are applied when
         * selecting this completion. Edits must not overlap (including the same
         * insert position) with the main edit nor with themselves.
         *
         * Additional text edits should be used to change text unrelated to the
         * current cursor position (for example adding an import statement at the
         * top of the file if the completion item will insert an unqualified type).
         */
        std::optional<std::vector<TextEdit>> additionalTextEdits;

        /**
         * An optional set of characters that when pressed while this completion is
         * active will accept it first and then type that character. *Note* that all
         * commit characters should have @code length=1@endcode and that superfluous characters
         * will be ignored.
         */
        std::optional<std::vector<string>> commitCharacters;

        /**
         * An optional command that is executed *after* inserting this completion.
         * *Note* that additional modifications to the current document should be
         * described with the additionalTextEdits-property.
         */
        std::optional<Command> command;

        /**
         * A data entry field that is preserved on a completion item between
         * a completion and a completion resolve request.
         */
        std::optional<LSPAny> data;
};

/**
 * Defines how values from a set of defaults and an individual item will be
 * merged.
 *
 * @version 3.18.0
 */
enum class ApplyKind {
        /**
         * The value from the individual item (if provided and not @code null@endcode) will be
         * used instead of the default.
         */
        Replace = 1,

        /**
         * The value from the item will be merged with the default.
         *
         * The specific rules for merging values are defined against each field
         * that supports merging.
         */
        Merge   = 1

};

/**
 * Represents a collection of [completion items](#CompletionItem) to be
 * presented in the editor.
 */
struct CompletionList {
        struct _ItemDefaults {
                struct _EditRange {
                        Range insert;
                        Range replace;
                };

                /**
                 * A default commit character set.
                 *
                 * @version 3.17.0
                 */
                std::optional<string> commitCharacters;

                /**
                 * A default edit range
                 *
                 * @version 3.17.0
                 */
                std::optional<std::variant<Range, _EditRange>> editRange;

                /**
                 * A default insert text format
                 *
                 * @version 3.17.0
                 */
                std::optional<InsertTextFormat> insertTextFormat;

                /**
                 * A default insert text mode
                 *
                 * @version 3.17.0
                 */
                std::optional<InsertTextMode> insertTextMode;

                /**
                 * A default data value.
                 *
                 * @version 3.17.0
                 */
                std::optional<LSPAny> data;
        };

        struct _ApplyKind {
                /**
                 * Specifies whether commitCharacters on a completion will replace or be
                 * merged with those in @code completionList.itemDefaults.commitCharacters@endcode.
                 *
                 * If ApplyKind.Replace, the commit characters from the completion item
                 * will always be used unless not provided, in which case those from
                 * @code completionList.itemDefaults.commitCharacters@endcode will be used. An
                 * empty list can be used if a completion item does not have any commit
                 * characters and also should not use those from
                 * @code completionList.itemDefaults.commitCharacters@endcode.
                 *
                 * If ApplyKind.Merge the commitCharacters for the completion will be
                 * the union of all values in both
                 * @code completionList.itemDefaults.commitCharacters@endcode and the completion's
                 * own @code commitCharacters@endcode.
                 *
                 * @version 3.18.0
                 */
                std::optional<ApplyKind> commitCharacters;

                /**
                 * Specifies whether the @code data@endcode field on a completion will replace or
                 * be merged with data from @code completionList.itemDefaults.data@endcode.
                 *
                 * If ApplyKind.Replace, the data from the completion item will be used
                 * if provided (and not @code null@endcode), otherwise
                 * @code completionList.itemDefaults.data@endcode will be used. An empty object can
                 * be used if a completion item does not have any data but also should
                 * not use the value from @code completionList.itemDefaults.data@endcode.
                 *
                 * If ApplyKind.Merge, a shallow merge will be performed between
                 * @code completionList.itemDefaults.data@endcode and the completion's own data
                 * using the following rules:
                 *
                 * - If a completion's @code data@endcode field is not provided (or @code null@endcode), the
                 *   entire @code data@endcode field from @code completionList.itemDefaults.data@endcode will be
                 *   used as-is.
                 * - If a completion's @code data@endcode field is provided, each field will
                 *   overwrite the field of the same name in
                 *   @code completionList.itemDefaults.data@endcode but no merging of nested fields
                 *   within that value will occur.
                 *
                 * @version 3.18.0
                 */
                std::optional<ApplyKind> data;
        };

        /**
         * This list is not complete. Further typing should result in recomputing
         * this list.
         *
         * Recomputed lists have all their items replaced (not appended) in the
         * incomplete completion sessions.
         */
        boolean isIncomplete;

        /**
         * In many cases the items of an actual completion result share the same
         * value for properties like @code commitCharacters@endcode or the range of a text
         * edit. A completion list can therefore define item defaults which will
         * be used if a completion item itself doesn't specify the value.
         *
         * If a completion list specifies a default value and a completion item
         * also specifies a corresponding value the one from the item is used.
         *
         * Servers are only allowed to return default values if the client
         * signals support for this via the @code completionList.itemDefaults@endcode
         * capability.
         *
         * @version 3.17.0
         */
        std::optional<_ItemDefaults> itemDefaults;

        /**
         * Specifies how fields from a completion item should be combined with those
         * from @code completionList.itemDefaults@endcode.
         *
         * If unspecified, all fields will be treated as ApplyKind.Replace.
         *
         * If a field's value is ApplyKind.Replace, the value from a completion item
         * (if provided and not @code null@endcode) will always be used instead of the value
         * from @code completionItem.itemDefaults@endcode.
         *
         * If a field's value is ApplyKind.Merge, the values will be merged using
         * the rules defined against each field below.
         *
         * Servers are only allowed to return @code applyKind@endcode if the client
         * signals support for this via the @code completionList.applyKindSupport@endcode
         * capability.
         *
         * @version 3.18.0
         */
        std::optional<_ApplyKind> applyKind;

        /**
         * The completion items.
         */
        std::vector<CompletionItem> items;
};

enum class DiagnosticSeverity {
        /**
         * Reports an error.
         */
        Error = 1,

        /**
         * Reports a warning.
         */
        Warning = 2,

        /**
         * Reports an information.
         */
        Information = 3,

        /**
         * Reports a hint.
         */
        Hint = 4

};

/**
 * Structure to capture a description for an error code.
 *
 * @version 3.16.0
 */
struct CodeDescription {
        /**
         * An URI to open with more information about the diagnostic error.
         */
        URI href;
};

/**
 * Represents a related message and source code location for a diagnostic.
 * This should be used to point to code locations that cause or are related to
 * a diagnostics, e.g when duplicating a symbol in a scope.
 */
struct DiagnosticRelatedInformation {
        /**
         * The location of this related diagnostic information.
         */
        Location location;

        /**
         * The message of this related diagnostic information.
         */
        string message;
};

struct Diagnostic {
        /**
         * The range at which the message applies.
         */
        Range range;

        /**
         * The diagnostic's severity. To avoid interpretation mismatches when a
         * server is used with different clients it is highly recommended that
         * servers always provide a severity value. If omitted, it's recommended
         * for the client to interpret it as an Error severity.
         */
        std::optional<DiagnosticSeverity> severity;

        /**
         * The diagnostic's code, which might appear in the user interface.
         */
        std::optional<std::variant<integer, string>> code;

        /**
         * An optional property to describe the error code.
         *
         * @version 3.16.0
         */
        std::optional<CodeDescription> codeDescription;

        /**
	 * A human-readable string describing the source of this
	 * diagnostic, e.g. 'typescript' or 'super lint'.
	 */
        std::optional<string> source;

        /**
         * The diagnostic's message.
         *
         * @version 3.18.0 - support for MarkupContent. This is guarded by the client
         *  capability @code textDocument.diagnostic.markupMessageSupport@endcode.
         */
        std::variant<string, MarkupContent> message;

        /**
         * Additional metadata about the diagnostic.
         *
         * @version 3.15.0
         */
        std::optional<std::vector<DiagnosticTag>> tags;

        /**
         * An array of related diagnostic information, e.g. when symbol-names within
         * a scope collide all definitions can be marked via this property.
         */
        std::optional<std::vector<DiagnosticRelatedInformation>> relatedInformation;

        /**
         * A data entry field that is preserved between a
         * @code textDocument/publishDiagnostics@endcode notification and
         * @code textDocument/codeAction@endcode request.
         *
         * @version 3.16.0
         */
        std::optional<LSPAny> data;
};

struct PublishDiagnosticsParams {
        /**
         * The URI for which diagnostic information is reported.
         */
        DocumentUri uri;

        /**
         * Optional the version number of the document the diagnostics are published
         * for.
         *
         * @version 3.15.0
         */
        std::optional<integer> version;

        /**
         * An array of diagnostic information items.
         */
        std::vector<Diagnostic> diagnostics;
};

/**
 * Parameters of the document diagnostic request.
 *
 * @version 3.17.0
 */
struct DocumentDiagnosticParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The additional identifier  provided during registration.
         */
        std::optional<string> identifier;

        /**
         * The result id of a previous response if provided.
         */
        std::optional<string> previousResultId;
};

STRING_ENUM(DocumentDiagnosticReportKind,
        Full, "full",
        Unchanged, "unchanged"
)

/**
 * A diagnostic report with a full set of problems.
 *
 * @version 3.17.0
 */
struct FullDocumentDiagnosticReport {
        /**
         * A full document diagnostic report.
         */
        static constexpr DocumentDiagnosticReportKind kind = DocumentDiagnosticReportKind::Full;

        /**
         * An optional result id. If provided it will
         * be sent on the next diagnostic request for the
         * same document.
         */
        std::optional<string> resultId;

        /**
         * The actual items.
         */
        std::vector<Diagnostic> items;
};

/**
 * A diagnostic report indicating that the last returned
 * report is still accurate.
 *
 * @version 3.17.0
 */
struct UnchangedDocumentDiagnosticReport {
        /**
         * A document diagnostic report indicating
         * no changes to the last result. A server can
         * only return @code unchanged@endcode if result ids are
         * provided.
         */
        static constexpr DocumentDiagnosticReportKind kind = DocumentDiagnosticReportKind::Unchanged;

        /**
         * A result id which will be sent on the next
         * diagnostic request for the same document.
         */
        string resultId;
};

/**
 * A full diagnostic report with a set of related documents.
 *
 * @version 3.17.0
 */
struct RelatedFullDocumentDiagnosticReport : FullDocumentDiagnosticReport {
        /**
         * Diagnostics of related documents. This information is useful
         * in programming languages where code in a file A can generate
         * diagnostics in a file B which A depends on. An example of
         * such a language is C/C++ where macro definitions in a file
         * a.cpp and result in errors in a header file b.hpp.
         *
         * @version 3.17.0
         */
        std::optional<std::unordered_map<DocumentUri, std::variant<FullDocumentDiagnosticReport, UnchangedDocumentDiagnosticReport>>> relatedDocuments;
};

/**
 * An unchanged diagnostic report with a set of related documents.
 *
 * @version 3.17.0
 */
struct RelatedUnchangedDocumentDiagnosticReport : UnchangedDocumentDiagnosticReport {
        /**
         * Diagnostics of related documents. This information is useful
         * in programming languages where code in a file A can generate
         * diagnostics in a file B which A depends on. An example of
         * such a language is C/C++ where macro definitions in a file
         * a.cpp and result in errors in a header file b.hpp.
         *
         * @version 3.17.0
         */
        std::optional<std::unordered_map<DocumentUri, std::variant<FullDocumentDiagnosticReport, UnchangedDocumentDiagnosticReport>>> relatedDocuments;
};

/**
 * The result of a document diagnostic pull request. A report can
 * either be a full report containing all diagnostics for the
 * requested document or a unchanged report indicating that nothing
 * has changed in terms of diagnostics in comparison to the last
 * pull request.
 *
 * @version 3.17.0
 */
using DocumentDiagnosticReport = std::variant<RelatedFullDocumentDiagnosticReport, RelatedUnchangedDocumentDiagnosticReport>;

/**
 * A partial result for a document diagnostic report.
 *
 * @version 3.17.0
 */
struct DocumentDiagnosticReportPartialResult {
        std::optional<std::unordered_map<DocumentUri, std::variant<FullDocumentDiagnosticReport, UnchangedDocumentDiagnosticReport>>> relatedDocuments;
};

/**
 * Cancellation data returned from a diagnostic request.
 *
 * @version 3.17.0
 */
struct DiagnosticServerCancellationData {
        boolean retriggerRequest;
};

/**
 * A previous result id in a workspace pull request.
 *
 * @version 3.17.0
 */
struct PreviousResultId {
        /**
         * The URI for which the client knows a
         * result id.
         */
        DocumentUri uri;

        /**
         * The value of the previous result id.
         */
        string value;
};

/**
 * Parameters of the workspace diagnostic request.
 *
 * @version 3.17.0
 */
struct WorkspaceDiagnosticParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The additional identifier provided during registration.
         */
        std::optional<string> identifier;

        /**
         * The currently known diagnostic reports with their
         * previous result ids.
         */
        std::vector<PreviousResultId> previousResultIds;
};

/**
 * A full document diagnostic report for a workspace diagnostic result.
 *
 * @version 3.17.0
 */
struct WorkspaceFullDocumentDiagnosticReport : FullDocumentDiagnosticReport {
        /**
         * The URI for which diagnostic information is reported.
         */
        DocumentUri uri;

        /**
         * The version number for which the diagnostics are reported.
         * If the document is not marked as open @code null@endcode can be provided.
         */
        std::variant<null, integer> version;
};

/**
 * An unchanged document diagnostic report for a workspace diagnostic result.
 *
 * @version 3.17.0
 */
struct WorkspaceUnchangedDocumentDiagnosticReport : UnchangedDocumentDiagnosticReport {
        /**
         * The URI for which diagnostic information is reported.
         */
        DocumentUri uri;

        /**
         * The version number for which the diagnostics are reported.
         * If the document is not marked as open @code null@endcode can be provided.
         */
        std::variant<null, integer> version;
};

/**
 * A workspace diagnostic document report.
 *
 * @version 3.17.0
 */
using WorkspaceDocumentDiagnosticReport = std::variant<WorkspaceFullDocumentDiagnosticReport, WorkspaceUnchangedDocumentDiagnosticReport>;

/**
 * A workspace diagnostic report.
 *
 * @version 3.17.0
 */
struct WorkspaceDiagnosticReport {
        std::vector<WorkspaceDocumentDiagnosticReport> items;
};

/**
 * A partial result for a workspace diagnostic report.
 *
 * @version 3.17.0
 */
struct WorkspaceDiagnosticReportPartialResult {
        std::vector<WorkspaceDocumentDiagnosticReport> items;
};

/**
 * How a signature help was triggered.
 *
 * @version 3.15.0
 */
enum class SignatureHelpTriggerKind {
        /**
         * Signature help was invoked manually by the user or by a command.
         */
        Invoked = 1,

        /**
         * Signature help was triggered by a trigger character.
         */
        TriggerCharacter = 2,

        /**
         * Signature help was triggered by the cursor moving or by the document
         * content changing.
         */
        ContentChange = 3

};

/**
 * Represents a parameter of a callable-signature. A parameter can
 * have a label and a doc-comment.
 */
struct ParameterInformation {
        /**
         * The label of this parameter information.
         *
         * Either a string or an inclusive start and exclusive end offsets within
         * its containing signature label. (see SignatureInformation.label). The
         * offsets are based on a UTF-16 string representation as @code Position@endcode and
         * @code Range@endcode does.
         *
         * @note a label of type string should be a substring of its containing
         * signature label. Its intended use case is to highlight the parameter
         * label part in the @code SignatureInformation.label@endcode.
         */
        std::variant<string, std::pair<uinteger, uinteger>> label;

        /**
         * The human-readable doc-comment of this parameter. Will be shown
         * in the UI but can be omitted.
         */
        std::optional<std::variant<string, MarkupContent>> documentation;
};

/**
 * Represents the signature of something callable. A signature
 * can have a label, like a function-name, a doc-comment, and
 * a set of parameters.
 */
struct SignatureInformation {
        /**
         * The label of this signature. Will be shown in
         * the UI.
         */
        string label;

        /**
         * The human-readable doc-comment of this signature. Will be shown
         * in the UI but can be omitted.
         */
        std::optional<std::variant<string, MarkupContent>> documentation;

        /**
         * The parameters of this signature.
         */
        std::optional<std::vector<ParameterInformation>> parameters;

        /**
         * The index of the active parameter.
         *
         * If provided, this is used in place of @code SignatureHelp.activeParameter@endcode.
         *
         * @version 3.16.0
         */
        std::optional<uinteger> activeParameter;
};

/**
 * Signature help represents the signature of something
 * callable. There can be multiple signature but only one
 * active and only one active parameter.
 */
struct SignatureHelp {
        /**
         * One or more signatures. If no signatures are available the signature help
         * request should return @code null@endcode.
         */
        std::vector<SignatureInformation> signatures;

        /**
         * The active signature. If omitted or the value lies outside the
         * range of @code signatures@endcode the value defaults to zero or is ignore if
         * the @code SignatureHelp@endcode as no signatures.
         *
         * Whenever possible implementors should make an active decision about
         * the active signature and shouldn't rely on a default value.
         *
         * In future version of the protocol this property might become
         * mandatory to better express this.
         */
        std::optional<uinteger> activeSignature;

        /**
         * The active parameter of the active signature. If omitted or the value
         * lies outside the range of @code signatures[activeSignature].parameters@endcode
         * defaults to 0 if the active signature has parameters. If
         * the active signature has no parameters it is ignored.
         * In future version of the protocol this property might become
         * mandatory to better express the active parameter if the
         * active signature does have any.
         */
        std::optional<uinteger> activeParameter;
};

/**
 * Additional information about the context in which a signature help request
 * was triggered.
 *
 * @version 3.15.0
 */
struct SignatureHelpContext {
        SignatureHelpTriggerKind triggerKind;

        std::optional<string> triggerCharacter;

        boolean isRetrigger;

        std::optional<SignatureHelp> activeSignatureHelp;
};

struct SignatureHelpParams : TextDocumentPositionParams, WorkDoneProgressParams {
        /**
         * The signature help context. This is only available if the client
         * specifies to send this using the client capability
         * @code textDocument.signatureHelp.contextSupport === true@endcode
         *
         * @version 3.15.0
         */
        std::optional<SignatureHelpContext> context;
};

/**
 * The reason why code actions were requested.
 *
 * @version 3.17.0
 */
enum class CodeActionTriggerKind {
        /**
         * Code actions were explicitly requested by the user or by an extension.
         */
        Invoked = 1,

        /**
         * Code actions were requested automatically.
         *
         * This typically happens when current selection in a file changes, but can
         * also be triggered when file content changes.
         */
        Automatic = 2

};

/**
 * Contains additional diagnostic information about the context in which
 * a code action is run.
 */
struct CodeActionContext {
        /**
         * An array of diagnostics known on the client side overlapping the range
         * provided to the @code textDocument/codeAction@endcode request. They are
         * provided so that the server knows which errors are currently presented
         * to the user for the given range. There is no guarantee that these
         * accurately reflect the error state of the resource. The primary parameter
         * to compute code actions is the provided range.
         */
        std::vector<Diagnostic> diagnostics;

        /**
         * Requested kind of actions to return.
         *
         * Actions not of this kind are filtered out by the client before being
         * shown. So servers can omit computing them.
         */
        std::optional<CodeActionKind> only;

        /**
         * The reason why code actions were requested.
         *
         * @version 3.17.0
         */
        std::optional<CodeActionTriggerKind> triggerKind;
};

/**
 * Params for the CodeActionRequest
 */
struct CodeActionParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The document in which the command was invoked.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The range for which the command was invoked.
         */
        Range range;

        /**
         * Context carrying additional information.
         */
        CodeActionContext context;
};

struct OptionalVersionedTextDocumentIdentifier : TextDocumentIdentifier {
        /**
         * The version number of this document. If an optional versioned text document
         * identifier is sent from the server to the client and the file is not
         * open in the editor (the server has not received an open notification
         * before) the server can send @code null@endcode to indicate that the version is
         * known and the content on disk is the master (as specified with document
         * content ownership).
         *
         * The version number of a document will increase after each change,
         * including undo/redo. The number doesn't need to be consecutive.
         */
        std::variant<null, integer> version;
};

/**
 * An identifier referring to a change annotation managed by a workspace
 * edit.
 *
 * @version 3.16.0.
 */
using ChangeAnnotationIdentifier = string;

/**
 * A special text edit with an additional change annotation.
 *
 * @version 3.16.0.
 */
struct AnnotatedTextEdit : TextEdit {
        /**
         * The actual annotation identifier.
         */
        ChangeAnnotationIdentifier annotationId;
};

/**
 * A string value used as a snippet is a template which allows to insert text
 * and to control the editor cursor when insertion happens.
 *
 * A snippet can define tab stops and placeholders with @code $1@endcode, @code $2@endcode
 * and @code ${3:foo}@endcode. @code $0@endcode defines the final tab stop, it defaults to
 * the end of the snippet. Variables are defined with @code $name@endcode and
 * @code ${name:default value}@endcode.
 *
 * @version 3.18.0
 */
struct StringValue {
        /**
         * The kind of string value.
         */
        static constexpr std::string_view kind = "snippet";

        /**
         * The snippet string.
         */
        string value;
};

/**
 * An interactive text edit.
 *
 * @version 3.18.0
 */
struct SnippetTextEdit {
        /**
         * The range of the text document to be manipulated.
         */
        Range range;

        /**
         * The snippet to be inserted.
         */
        StringValue snippet;

        /**
         * The actual identifier of the snippet edit.
         */
        ChangeAnnotationIdentifier annotationId;
};

struct TextDocumentEdit {
        /**
         * The text document to change.
         */
        OptionalVersionedTextDocumentIdentifier textDocument;

        /**
         * The edits to be applied.
         *
         * @version 3.16.0 - support for AnnotatedTextEdit. This is guarded by the
         *  client capability @code workspace.workspaceEdit.changeAnnotationSupport@endcode
         *
         * @version 3.18.0 - support for SnippetTextEdit. This is guarded by the
         *  client capability @code workspace.workspaceEdit.snippetEditSupport@endcode
         */
        std::vector<std::variant<TextEdit, AnnotatedTextEdit, SnippetTextEdit>> edits;
};

/**
 * Options to create a file.
 */
struct CreateFileOptions {
        /**
         * Overwrite existing file. Overwrite wins over @code ignoreIfExists@endcode
         */
        std::optional<boolean> overwrite;

        /**
         * Ignore if exists.
         */
        std::optional<boolean> ignoreIfExists;
};

/**
 * Create file operation
 */
struct CreateFile {
        /**
         * A create
         */
        static constexpr std::string_view kind = "create";

        /**
         * The resource to create.
         */
        DocumentUri uri;

        /**
         * Additional options
         */
        std::optional<CreateFileOptions> options;

        /**
         * An optional annotation identifier describing the operation.
         *
         * @version 3.16.0
         */
        std::optional<ChangeAnnotationIdentifier> annotationId;
};

/**
 * Rename file options
 */
struct RenameFileOptions {
        /**
         * Overwrite target if existing. Overwrite wins over @code ignoreIfExists@endcode
         */
        std::optional<boolean> overwrite;

        /**
         * Ignore if target exists.
         */
        std::optional<boolean> ignoreIfExists;
};

/**
 * Rename file operation
 */
struct RenameFile {
        /**
         * A rename
         */
        static constexpr std::string_view kind = "rename";

        /**
         * The old (existing) location.
         */
        DocumentUri oldUri;

        /**
         * The new location.
         */
        DocumentUri newUri;

        /**
         * Rename options.
         */
        std::optional<RenameFileOptions> options;

        /**
         * An optional annotation identifier describing the operation.
         *
         * @version 3.16.0
         */
        std::optional<ChangeAnnotationIdentifier> annotationId;
};

/**
 * Delete file options
 */
struct DeleteFileOptions {
        /**
         * Delete the content recursively if a folder is denoted.
         */
        std::optional<boolean> recursive;

        /**
         * Ignore the operation if the file doesn't exist.
         */
        std::optional<boolean> ignoreIfNotExists;
};

/**
 * Delete file operation
 */
struct DeleteFile {
        /**
         * A delete
         */
        static constexpr std::string_view kind = "delete";

        /**
         * The file to delete.
         */
        DocumentUri uri;

        /**
         * Delete options.
         */
        std::optional<DeleteFileOptions> options;

        /**
         * An optional annotation identifier describing the operation.
         *
         * @version 3.16.0
         */
        std::optional<ChangeAnnotationIdentifier> annotationId;
};

/**
 * Additional information that describes document changes.
 *
 * @version 3.16.0
 */
struct ChangeAnnotation {
        /**
         * A human-readable string describing the actual change. The string
         * is rendered prominent in the user interface.
         */
        string label;

        /**
         * A flag which indicates that user confirmation is needed
         * before applying the change.
         */
        std::optional<boolean> needsConfirmation;

        /**
         * A human-readable string which is rendered less prominent in
         * the user interface.
         */
        std::optional<string> description;
};

struct WorkspaceEdit {
        /**
         * Holds changes to existing resources.
         */
        std::optional<std::unordered_map<DocumentUri, std::vector<TextEdit>>> changes;

        /**
         * Depending on the client capability
         * @code workspace.workspaceEdit.resourceOperations@endcode document changes
         * are either an array of @code TextDocumentEdit@endcode s to express changes
         * to n different text documents where each text document edit addresses a
         * specific version of a text document. Or it can contain above
         * @code TextDocumentEdit@endcode s mixed with create, rename and delete
         * file / folder operations.
         *
         * Whether a client supports versioned document edits is expressed via
         * @code workspace.workspaceEdit.documentChanges@endcode client capability.
         *
         * If a client neither supports @code documentChanges@endcode nor
         * @code workspace.workspaceEdit.resourceOperations@endcode then only plain
         * @code TextEdit@endcode s using the @code changes@endcode property are
         * supported.
         */
        std::optional<std::variant<std::vector<TextDocumentEdit>, std::vector<std::variant<TextDocumentEdit, CreateFile, RenameFile, DeleteFile>>>> documentChanges;

        /**
         * A map of change annotations that can be referenced in
         * @code AnnotatedTextEdit@endcode s or create, rename and delete file / folder
         * operations.
         *
         * Whether clients honor this property depends on the client capability
         * @code workspace.changeAnnotationSupport@endcode .
         *
         * @version 3.16.0
         */
        std::optional<std::unordered_map<ChangeAnnotationIdentifier, ChangeAnnotation>> changeAnnotations;
};

/**
 * A code action represents a change that can be performed in code, e.g. to fix
 * a problem or to refactor code.
 *
 * A CodeAction must set either @code edit@endcode and/or a @code command@endcode.
 * If both are supplied, the @code edit@endcode is applied first, then the
 * @code command@endcode is executed.
 */
struct CodeAction {
        struct _Disabled {
                /**
                 * Human readable description of why the code action is currently
                 * disabled.
                 *
                 * This is displayed in the code actions UI.
                 */
                string reason;
        };

        /**
         * A short, human-readable, title for this code action.
         */
        string title;

        /**
         * The kind of the code action.
         *
         * Used to filter code actions.
         */
        std::optional<CodeActionKind> kind;

        /**
         * The diagnostics that this code action resolves.
         */
        std::optional<std::vector<Diagnostic>> diagnostics;

        /**
         * Marks this as a preferred action. Preferred actions are used by the
         * @code auto fix@endcode command and can be targeted by keybindings.
         *
         * A quick fix should be marked preferred if it properly addresses the
         * underlying error. A refactoring should be marked preferred if it is the
         * most reasonable choice of actions to take.
         *
         * @version 3.15.0
         */
        std::optional<boolean> isPreferred;

        /**
         * Marks that the code action cannot currently be applied.
         *
         * Clients should follow the following guidelines regarding disabled code
         * actions:
         *
         * - Disabled code actions are not shown in automatic lightbulbs code
         *   action menus.
         * - Disabled actions are shown as faded out in the code action menu when
         *   the user request a more specific type of code action, such as
         *   refactorings.
         * - If the user has a keybinding that auto applies a code action and only
         *   a disabled code actions are returned, the client should show the user
         *   an error message with @code reason@endcode in the editor.
         *
         * @version 3.16.0
         */
        std::optional<_Disabled> disabled;

        /**
         * The workspace edit this code action performs.
         */
        std::optional<WorkspaceEdit> edit;

        /**
         * A command this code action executes. If a code action
         * provides an edit and a command, first the edit is
         * executed and then the command.
         */
        std::optional<Command> command;

        /**
         * A data entry field that is preserved on a code action between
         * a @code textDocument/codeAction@endcode and a
         * @code codeAction/resolve@endcode request.
         *
         * @version 3.16.0
         */
        std::optional<LSPAny> data;

        /**
          * Tags for this code action.
         *
         * @version 3.18.0 - proposed
         */
        std::optional<std::vector<CodeActionTag>> tags;
};

struct DocumentColorParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;
};

/**
 * Represents a color in RGBA space.
 */
struct Color {
        /**
         * The red component of this color in the range [0-1].
         */
        decimal red;

        /**
         * The green component of this color in the range [0-1].
         */
        decimal green;

        /**
         * The blue component of this color in the range [0-1].
         */
        decimal blue;

        /**
         * The alpha component of this color in the range [0-1].
         */
        decimal alpha;
};

struct ColorInformation {
        /**
         * The range in the document where this color appears.
         */
        Range range;

        /**
         * The actual color value for this color range.
         */
        Color color;
};

struct ColorPresentationParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * The text document.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The color information to request presentations for.
         */
        Color color;

        /**
         * The range where the color would be inserted. Serves as a context.
         */
        Range range;
};

struct ColorPresentation {
        /**
         * The label of this color presentation. It will be shown on the color
         * picker header. By default this is also the text that is inserted when
         * selecting this color presentation.
         */
        string label;

        /**
         * An [edit](#TextEdit) which is applied to a document when selecting
         * this presentation for the color. When omitted the
         * @link ColorPresentation::label label@endlink is used.
         */
        std::optional<TextEdit> textEdit;

        /**
         * An optional array of additional [text edits](#TextEdit) that are applied
         * when selecting this color presentation. Edits must not overlap with the
         * main @link ColorPresentation::textEdit edit@endlink nor with themselves.
         */
        std::optional<std::vector<TextEdit>> additionalTextEdits;
};

/**
 * Value-object describing what options formatting should use.
 */
struct FormattingOptions {
        /**
         * Size of a tab in spaces.
         */
        uinteger tabSize;

        /**
         * Prefer spaces over tabs.
         */
        boolean insertSpaces;

        /**
         * Trim trailing whitespace on a line.
         *
         * @version 3.15.0
         */
        std::optional<boolean> trimTrailingWhitespace;

        /**
         * Insert a newline character at the end of the file if one does not exist.
         *
         * @version 3.15.0
         */
        std::optional<boolean> insertFinalNewline;

        /**
         * Trim all newlines after the final newline at the end of the file.
         *
         * @version 3.15.0
         */
        std::optional<boolean> trimFinalNewlines;

        /**
         * Signature for further properties.
         */
        std::unordered_map<string, std::variant<boolean, integer, string>> extra;
};

struct DocumentFormattingParams : WorkDoneProgressParams {
        /**
         * The document to format.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The format options.
         */
        FormattingOptions options;
};

struct DocumentRangeFormattingParams : WorkDoneProgressParams {
        /**
         * The document to format.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The range to format
         */
        Range range;

        /**
         * The format options.
         */
        FormattingOptions options;
};

struct DocumentRangesFormattingParams : WorkDoneProgressParams {
        /**
         * The document to format.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The ranges to format
         */
        std::vector<Range> ranges;

        /**
         * The format options.
         */
        FormattingOptions options;
};

struct DocumentOnTypeFormattingParams {
        /**
         * The document to format.
         */
        TextDocumentIdentifier textDocument;

        /**
         * The position around which the on type formatting should happen.
         * This is not necessarily the exact position where the character denoted
         * by the property @code ch@endcode got typed.
         */
        Position position;

        /**
         * The character that has been typed that triggered the formatting
         * on type request. That is not necessarily the last character that
         * got inserted into the document since the client could auto insert
         * characters as well (e.g. like automatic brace completion).
         */
        string ch;

        /**
         * The formatting options.
         */
        FormattingOptions options;
};

struct RenameParams : TextDocumentPositionParams, WorkDoneProgressParams {
        /**
         * The new name of the symbol. If the given name is not valid the
         * request must return a [ResponseError](#ResponseError) with an
         * appropriate message set.
         */
        string newName;
};

struct PrepareRenameParams : TextDocumentPositionParams, WorkDoneProgressParams {
};

struct _PrepareRenameRequestResult1 {
        Range range;
        string placeholder;
};

struct _PrepareRenameRequestResult2 {
        boolean defaultBehavior;
};

struct LinkedEditingRangeParams : TextDocumentPositionParams, WorkDoneProgressParams {
};

struct LinkedEditingRanges {
        /**
         * A list of ranges that can be renamed together. The ranges must have
         * identical length and contain identical text content. The ranges cannot
         * overlap.
         */
        std::vector<Range> ranges;

        /**
         * An optional word pattern (regular expression) that describes valid
         * contents for the given ranges. If no pattern is provided, the client
         * configuration's word pattern will be used.
         */
        std::optional<string> wordPattern;
};

/**
 * Describes how an {@link InlineCompletionItemProvider inline completion
 * provider @endlink} was triggered.
 *
 * @version 3.18.0
 */
enum class InlineCompletionTriggerKind {
        /**
         * Completion was triggered explicitly by a user gesture.
         * Return multiple completion items to enable cycling through them.
         */
        Invoked   = 1,

        /**
         * Completion was triggered automatically while editing.
         * It is sufficient to return a single completion item in this case.
         */
        Automatic = 2

};

/**
 * Describes the currently selected completion item.
 *
 * @version 3.18.0
 */
struct SelectedCompletionInfo {
        /**
         * The range that will be replaced if this completion item is accepted.
         */
        Range range;

        /**
         * The text the range will be replaced with if this completion is
         * accepted.
         */
        string text;
};

/**
 * Provides information about the context in which an inline completion was
 * requested.
 *
 * @version 3.18.0
 */
struct InlineCompletionContext {
        /**
         * Describes how the inline completion was triggered.
         */
        InlineCompletionTriggerKind triggerKind;

        /**
         * Provides information about the currently selected item in the
         * autocomplete widget if it is visible.
         *
         * If set, provided inline completions must extend the text of the
         * selected item and use the same range, otherwise they are not shown as
         * preview.
         * As an example, if the document text is @code console.@endcode and the selected item
         * is @code .log@endcode replacing the @code .@endcode in the document, the inline completion must
         * also replace @code .@endcode and start with @code .log@endcode, for example @code .log()@endcode.
         *
         * Inline completion providers are requested again whenever the selected
         * item changes.
         */
        std::optional<SelectedCompletionInfo> selectedCompletionInfo;
};

/**
 * A parameter literal used in inline completion requests.
 *
 * @version 3.18.0
 */
struct InlineCompletionParams : TextDocumentPositionParams, WorkDoneProgressParams {
        /**
         * Additional information about the context in which inline completions
         * were requested.
         */
        InlineCompletionContext context;
};

/**
 * An inline completion item represents a text snippet that is proposed inline
 * to complete text that is being typed.
 *
 * @version 3.18.0
 */
struct InlineCompletionItem {
        /**
         * The text to replace the range with. Must be set.
         * Is used both for the preview and the accept operation.
         */
        std::variant<string, StringValue> insertText;

        /**
         * A text that is used to decide if this inline completion should be
         * shown. When @code falsy@endcode, the {@link InlineCompletionItem::insertText @endlink} is
         * used.
         *
         * An inline completion is shown if the text to replace is a prefix of the
         * filter text.
         */
        std::optional<string> filterText;

        /**
         * The range to replace.
         * Must begin and end on the same line.
         *
         * Prefer replacements over insertions to provide a better experience when
         * the user deletes typed text.
         */
        std::optional<Range> range;

        /**
         * An optional {@link Command @endlink} that is executed *after* inserting this
         * completion.
         */
        std::optional<Command> command;
};

/**
 * Represents a collection of {@link InlineCompletionItem inline completion
 * items @endlink} to be presented in the editor.
 *
 * @version 3.18.0
 */
struct InlineCompletionList {
        /**
         * The inline completion items.
         */
        std::vector<InlineCompletionItem> items;
};

/**
 * The parameters of a Workspace Symbol Request.
 */
struct WorkspaceSymbolParams : WorkDoneProgressParams, PartialResultParams {
        /**
         * A query string to filter symbols by. Clients may send an empty
         * string here to request all symbols.
         */
        string query;
};

/**
 * A special workspace symbol that supports locations without a range
 *
 * @version 3.17.0
 */
struct WorkspaceSymbol {
        struct _Location {
                DocumentUri uri;
        };

        /**
         * The name of this symbol.
         */
        string name;

        /**
         * The kind of this symbol.
         */
        SymbolKind kind;

        /**
         * Tags for this completion item.
         */
        std::optional<std::vector<SymbolTag>> tags;

        /**
         * The name of the symbol containing this symbol. This information is for
         * user interface purposes (e.g. to render a qualifier in the user interface
         * if necessary). It can't be used to re-infer a hierarchy for the document
         * symbols.
         */
        std::optional<string> containerName;

        /**
         * The location of this symbol. Whether a server is allowed to
         * return a location without a range depends on the client
         * capability @code workspace.symbol.resolveSupport@endcode.
         *
         * See also @code SymbolInformation.location@endcode.
         */
        std::variant<Location, _Location> location;

        /**
         * A data entry field that is preserved on a workspace symbol between a
         * workspace symbol request and a workspace symbol resolve request.
         */
        std::optional<LSPAny> data;
};

struct ConfigurationItem {
        /**
         * The scope to get the configuration section for.
         */
        std::optional<URI> scopeUri;

        /**
         * The configuration section asked for.
         */
        std::optional<string> section;
};

struct ConfigurationParams {
        std::vector<ConfigurationItem> items;
};

struct DidChangeConfigurationParams {
        /**
         * The actual changed settings
         */
        LSPAny settings;
};

/**
 * The workspace folder change event.
 */
struct WorkspaceFoldersChangeEvent {
        /**
         * The array of added workspace folders
         */
        std::vector<WorkspaceFolder> added;

        /**
         * The array of the removed workspace folders
         */
        std::vector<WorkspaceFolder> removed;
};

struct DidChangeWorkspaceFoldersParams {
        /**
         * The actual workspace folder change event.
         */
        WorkspaceFoldersChangeEvent event;
};

/**
 * Represents information on a file/folder create.
 *
 * @version 3.16.0
 */
struct FileCreate {
        /**
         * A file:// URI for the location of the file/folder being created.
         */
        string uri;
};

/**
 * The parameters sent in notifications/requests for user-initiated creation
 * of files.
 *
 * @version 3.16.0
 */
struct CreateFilesParams {
        /**
         * An array of all files/folders created in this operation.
         */
        std::vector<FileCreate> files;
};

/**
 * Represents information on a file/folder rename.
 *
 * @version 3.16.0
 */
struct FileRename {
        /**
         * A file:// URI for the original location of the file/folder being renamed.
         */
        string oldUri;

        /**
         * A file:// URI for the new location of the file/folder being renamed.
         */
        string newUri;
};

/**
 * The parameters sent in notifications/requests for user-initiated renames
 * of files.
 *
 * @version 3.16.0
 */
struct RenameFilesParams {
        /**
         * An array of all files/folders renamed in this operation. When a folder
         * is renamed, only the folder will be included, and not its children.
         */
        std::vector<FileRename> files;
};

/**
 * Represents information on a file/folder delete.
 *
 * @version 3.16.0
 */
struct FileDelete {
        /**
         * A file:// URI for the location of the file/folder being deleted.
         */
        string uri;
};

/**
 * The parameters sent in notifications/requests for user-initiated deletes
 * of files.
 *
 * @version 3.16.0
 */
struct DeleteFilesParams {
        /**
         * An array of all files/folders deleted in this operation.
         */
        std::vector<FileDelete> files;
};

/**
 * The file event type.
 */
enum class FileChangeType {
        /**
         * The file got created.
         */
        Created = 1,

        /**
         * The file got changed.
         */
        Changed = 2,

        /**
         * The file got deleted.
         */
        Deleted = 3

};

/**
 * An event describing a file change.
 */
struct FileEvent {
        /**
         * The file's URI.
         */
        DocumentUri uri;

        /**
         * The change type.
         */
        FileChangeType type;
};

struct DidChangeWatchedFilesParams {
        /**
         * The actual file events.
         */
        std::vector<FileEvent> changes;
};

struct ExecuteCommandParams : WorkDoneProgressParams {
        /**
         * The identifier of the actual command handler.
         */
        string command;

        /**
         * Arguments that the command should be invoked with.
         */
        std::optional<std::vector<LSPAny>> arguments;
};

/**
 * Additional data about a workspace edit.
 *
 * @version 3.18.0
 * @proposed
 */
struct WorkspaceEditMetadata {
        /**
         * Signal to the editor that this edit is a refactoring.
         */
        std::optional<boolean> isRefactoring;
};

struct ApplyWorkspaceEditParams {
        /**
         * An optional label of the workspace edit. This label is
         * presented in the user interface for example on an undo
         * stack to undo the workspace edit.
         */
        std::optional<string> label;

        /**
         * The edits to apply.
         */
        WorkspaceEdit edit;

        /**
         * Additional data about the edit.
         *
         * @version 3.18.0
         * @proposed
         */
        std::optional<WorkspaceEditMetadata> metadata;
};

struct ApplyWorkspaceEditResult {
        /**
         * Indicates whether the edit was applied or not.
         */
        boolean applied;

        /**
         * An optional textual description for why the edit was not applied.
         * This may be used by the server for diagnostic logging or to provide
         * a suitable error for a request that triggered the edit.
         */
        std::optional<string> failureReason;

        /**
         * Depending on the client's failure handling strategy @code failedChange@endcode
         * might contain the index of the change that failed. This property is
         * only available if the client signals a @code failureHandling@endcode strategy
         * in its client capabilities.
         */
        std::optional<uinteger> failedChange;
};

/**
 * Parameters for the @code workspace/textDocumentContent@endcode request.
 *
 * @version 3.18.0
 */
struct TextDocumentContentParams {
        /**
         * The uri of the text document.
         */
        DocumentUri uri;
};

/**
 * Result of the @code workspace/textDocumentContent@endcode request.
 *
 * @version 3.18.0 - proposed
 */
struct TextDocumentContentResult {
        /**
         * The text content of the text document. Please note, that the content of
         * any subsequent open notifications for the text document might differ
         * from the returned content due to whitespace and line ending
         * normalizations done on the client
         */
        string text;
};

/**
 * Parameters for the @code workspace/textDocumentContent/refresh@endcode request.
 *
 * @version 3.18.0
 */
struct TextDocumentContentRefreshParams {
        /**
         * The uri of the text document to refresh.
         */
        DocumentUri uri;
};

enum class MessageType {
        /**
         * An error message.
         */
        Error = 1,

        /**
         * A warning message.
         */
        Warning = 2,

        /**
         * An information message.
         */
        Info = 3,

        /**
         * A log message.
         */
        Log = 4,

        /**
         * A debug message.
         *
         * @version 3.18.0 - proposed
         */
        Debug = 5

};

struct ShowMessageParams {
        /**
         * The message type. See {@link MessageType @endlink}.
         */
        MessageType type;

        /**
         * The actual message.
         */
        string message;
};

struct MessageActionItem {
        /**
         * A short title like 'Retry', 'Open Log' etc.
         */
        string title;
};

struct ShowMessageRequestParams {
        /**
         * The message type. See {@link MessageType @endlink}.
         */
        MessageType type;

        /**
         * The actual message.
         */
        string message;

        /**
         * The message action items to present.
         */
        std::optional<std::vector<MessageActionItem>> actions;
};

/**
 * Params to show a resource.
 *
 * @version 3.16.0
 */
struct ShowDocumentParams {
        /**
         * The uri to show.
         */
        URI uri;

        /**
         * Indicates to show the resource in an external program.
         * To show, for example, @code https://code.visualstudio.com/@endcode
         * in the default WEB browser set @code external@endcode to @code true@endcode.
         */
        std::optional<boolean> external;

        /**
         * An optional property to indicate whether the editor
         * showing the document should take focus or not.
         * Clients might ignore this property if an external
         * program is started.
         */
        std::optional<boolean> takeFocus;

        /**
         * An optional selection range if the document is a text
         * document. Clients might ignore the property if an
         * external program is started or the file is not a text
         * file.
         */
        std::optional<Range> selection;
};

/**
 * The result of an show document request.
 *
 * @version 3.16.0
 */
struct ShowDocumentResult {
        /**
         * A boolean indicating if the show was successful.
         */
        boolean success;
};

struct LogMessageParams {
        /**
         * The message type. See {@link MessageType @endlink}
         */
        MessageType type;

        /**
         * The actual message
         */
        string message;
};

struct WorkDoneProgressCreateParams {
        /**
         * The token to be used to report progress.
         */
        ProgressToken token;
};

struct WorkDoneProgressCancelParams {
        /**
         * The token to be used to report progress.
         */
        ProgressToken token;
};

enum class ErrorCodes {
        ParseError     = -32700,
        InvalidRequest = -32600,
        MethodNotFound = -32601,
        InvalidParams  = -32602,
        InternalError  = -32603,

        /**
         * This is the start range of JSON-RPC reserved error codes.
         * It doesn't denote a real error code. No LSP error codes should
         * be defined between the start and end range. For backwards
         * compatibility the @code ServerNotInitialized@endcode and the
         * @code UnknownErrorCode@endcode are left in the range.
         *
         * @version 3.16.0
         */
        jsonrpcReservedErrorRangeStart = -32099,
        /** @deprecated use jsonrpcReservedErrorRangeStart */
        serverErrorStart               = jsonrpcReservedErrorRangeStart,

        /**
         * Error code indicating that a server received a notification or
         * request before the server received the @code initialize@endcode request.
         */
        ServerNotInitialized = -32002,
        UnknownErrorCode     = -32001,

        /**
         * This is the end range of JSON-RPC reserved error codes.
         * It doesn't denote a real error code.
         *
         * @version 3.16.0
         */
        jsonrpcReservedErrorRangeEnd = -32000,
        /** @deprecated use jsonrpcReservedErrorRangeEnd */
        serverErrorEnd               = jsonrpcReservedErrorRangeEnd,

        /**
         * This is the start range of LSP reserved error codes.
         * It doesn't denote a real error code.
         *
         * @version 3.16.0
         */
        lspReservedErrorRangeStart = -32899,

        /**
         * A request failed but it was syntactically correct, e.g the
         * method name was known and the parameters were valid. The error
         * message should contain human readable information about why
         * the request failed.
         *
         * @version 3.17.0
         */
        RequestFailed = -32803,

        /**
         * The server cancelled the request. This error code should
         * only be used for requests that explicitly support being
         * server cancellable.
         *
         * @version 3.17.0
         */
        ServerCancelled = -32802,

        /**
         * The server detected that the content of a document got
         * modified outside normal conditions. A server should
         * NOT send this error code if it detects a content change
         * in its unprocessed messages. The result even computed
         * on an older state might still be useful for the client.
         *
         * If a client decides that a result is not of any use anymore
         * the client should cancel the request.
         */
        ContentModified = -32801,

        /**
         * The client has canceled a request and a server has detected
         * the cancel.
         */
        RequestCancelled = -32800,

        /**
         * This is the end range of LSP reserved error codes.
         * It doesn't denote a real error code.
         *
         * @version 3.16.0
         */
        lspReservedErrorRangeEnd = -32800
};

/** Initialize Request
 *
 * The initialize request is sent as the first request from the client to the server.
 * If the server receives a request or notification before the
 * @code initialize@endcode request it should act as follows:
 *
 *  - For a request the response should be an error with @code code: -32002@endcode.
 *    The message can be picked by the server.
 *  - Notifications should be dropped, except for the exit notification. This will
 *    allow the exit of a server without an initialize request.
 *
 * Until the server has responded to the @code initialize@endcode request with an
 * @code InitializeResult@endcode, the client must not send any additional requests
 * or notifications to the server. In addition the server is not allowed to send an
 * requests or notifications to the client until it has responded with an
 * @code InitializeResult@endcode, with the exception that during the
 * @code initialize@endcode request the server is allowed to send the notifications
 * @code window/showMessage@endcode, @code window/logMessage@endcode and
 * @code telemetry/event@endcode as well as the @code window/showMessageRequest@endcode
 * request to the client. In case the client sets up a progress token in the
 * initialize params (e.g. property @code workDoneToken@endcode) the server is also
 * allowed to use that token (and only that token) using the @code $/progress@endcode
 * notification sent from the server to the client.
 *
 * The @code initialize@endcode request may only be sent once.
 */
using InitializeRequest = glz::rpc::method<
        "initialize",
        InitializeParams,
        InitializeResult
>;

/** Initialized Notification
 *
 * The initialized notification is sent from the client to the server after the
 * client received the result of the @code initialize@endcode request but before
 * the client is sending any other request or notification to the server.
 * The server can use the @code initialized@endcode notification, for example,
 * to dynamically register capabilities. The @code initialized@endcode notification
 * may only be sent once.
 */
using InitializedNotification = glz::rpc::method<
        "initialized",
        InitializedParams,
        _Void
>;

/** Register Capability
 *
 * The @code client/registerCapability@endcode request is sent from the server
 * to the client to register for a new capability on the client side. Not all clients
 * need to support dynamic capability registration. A client opts in via the
 * @code dynamicRegistration@endcode property on the specific client capabilities.
 * A client can even provide dynamic registration for capability A but not for
 * capability B (see @code TextDocumentClientCapabilities@endcode as an example).
 *
 * Server must not register the same capability both statically through the initialize
 * result and dynamically for the same document selector. If a server wants to
 * support both static and dynamic registration it needs to check the client
 * capability in the initialize request and only register the capability statically
 * if the client doesn't support dynamic registration for that capability.
 */
using RegisterCapability = glz::rpc::method<
        "client/registerCapability",
        RegistrationParams,
        _Void
>;

/** Unregister Capability
 *
 * The @code client/unregisterCapability@endcode request is sent from the server
 * to the client to unregister a previously registered capability.
 */
using UnregisterCapability = glz::rpc::method<
        "client/unregisterCapability",
        UnregistrationParams,
        _Void
>;

/** SetTrace Notification
 *
 * A notification that should be used by the client to modify the trace setting of the server.
 */
using SetTraceNotification = glz::rpc::method<
        "$/setTrace",
        SetTraceParams,
        _Void
>;

/** LogTrace Notification
 *
 * A notification to log the trace of the server's execution. The amount and content
 * of these notifications depends on the current @code trace@endcode configuration.
 * If @code trace@endcode is @code 'off'@endcode, the server should not send any
 * @code logTrace@endcode notification. If @code trace@endcode is @code 'messages'@endcode,
 * the server should not add the @code 'verbose'@endcode field in the
 * @code LogTraceParams@endcode.
 *
 * @code $/logTrace@endcode should be used for systematic trace reporting. For
 * single debugging messages, the server should send window/logMessage notifications.
 */
using LogTraceNotification = glz::rpc::method<
        "$/logTrace",
        LogTraceParams,
        _Void
>;

/** Shutdown Request
 *
 * The shutdown request is sent from the client to the server. It asks the server to
 * shut down, but to not exit (otherwise the response might not be delivered correctly
 * to the client). There is a separate exit notification that asks the server to exit.
 * Clients must not send any notifications other than @code exit@endcode or requests
 * to a server to which they have sent a shutdown request. Clients should also
 * wait with sending the @code exit@endcode notification until they have received
 * a response from the @code shutdown@endcode request.
 *
 * If a server receives requests after a shutdown request those requests should
 * error with @code InvalidRequest@endcode.
 */
using ShutdownRequest = glz::rpc::method<
        "shutdown",
        _Void,
        null
>;

/** Exit Notification
 *
 * A notification to ask the server to exit its process. The server should exit
 * with @code success@endcode code 0 if the shutdown request has been received
 * before; otherwise with @code error@endcode code 1.
 */
using ExitNotification = glz::rpc::method<
        "exit",
        _Void,
        _Void
>;

/** DidOpenTextDocument Notification
 *
 * The document open notification is sent from the client to the server to signal
 * newly opened text documents. The document's content is now managed by the client
 * and the server must not try to read the document's content using the document's Uri.
 * Open in this sense means it is managed by the client. It doesn't necessarily mean
 * that its content is presented in an editor. An open notification must not be
 * sent more than once without a corresponding close notification send before.
 * This means open and close notification must be balanced and the max open count
 * for a particular textDocument is one. Note that a server's ability to fulfill
 * requests is independent of whether a text document is open or closed.
 *
 * The @code DidOpenTextDocumentParams@endcode contain the language id the document
 * is associated with. If the language id of a document changes, the client needs
 * to send a @code textDocument/didClose@endcode to the server followed by a
 * @code textDocument/didOpen@endcode with the new language id if the server handles
 * the new language id as well.
 *
 * Client Capability:
 *  - property path (optional): @code textDocument.synchronization.dynamicRegistration@endcode
 *  - property type: @code boolean@endcode
 *
 * Server Capability:
 *  - property path (optional): @code textDocumentSync@endcode
 *  - property type: @code TextDocumentSyncKind | TextDocumentSyncOptions@endcode.
 */
using DidOpenTextDocumentNotification = glz::rpc::method<
        "textDocument/didOpen",
        DidOpenTextDocumentParams,
        _Void
>;

/** DidChangeTextDocument Notification
 *
 * The document change notification is sent from the client to the server to signal
 * changes to a text document. Before a client can change a text document it must
 * claim ownership of its content using the @code textDocument/didOpen@endcode
 * notification. In 2.0 the shape of the params has changed to include proper
 * version numbers.
 *
 * Before requesting information from the server (e.g., @code textDocument/completion@endcode
 * or @code textDocument/signatureHelp@endcode), the client must ensure that the
 * document's state is synchronized with the server to guarantee reliable results.
 *
 * The following example shows how the client should synchronize the state when the
 * user has continuous input, assuming user input triggered @code textDocument/completion@endcode:
 *
 * @verbatim
 * | -------------- | -----------| --------------------------------- | ------------- |
 * | Document       | User Input | Client Behaviour                  | Request       |
 * | Version        |            |                                   |               |
 * | -------------- | -----------| --------------------------------- | ------------- |
 * | 5              | document   | sync document v5 to the server    | textDocument/ |
 * |                | change one |                                   | didChange     |
 * | -------------- | -----------| --------------------------------- | ------------- |
 * | 5              | -          | request from the server, based on | textDocument/ |
 * |                |            | document v5                       | completion    |
 * | -------------- | -----------| --------------------------------- | ------------- |
 * | 6              | document   | synd document v6 to the server    | textDocument/ |
 * |                | change two |                                   | didChange     |
 * | -------------- | -----------| --------------------------------- | ------------- |
 * @endverbatim
 *
 * Client Capability:
 *  - property path (optional): @code textDocument.synchronization.dynamicRegistration@endcode
 *  - property type: @code boolean@endcode
 *
 * Server Capability:
 *  - property path (optional): @code textDocumentSync@endcode
 *  - property type: @code TextDocumentSyncKind | TextDocumentSyncOptions@endcode.
 *
 * Registration Options: @code TextDocumentChangeRegistrationOptions@endcode
 */
using DidChangeTextDocumentNotification = glz::rpc::method<
        "textDocument/didChange",
        DidChangeTextDocumentParams,
        _Void
>;

/** WillSaveTextDocument Notification
 *
 * The document will save notification is sent from the client to the server before
 * the document is actually saved. If a server has registered for open / close
 * events clients should ensure that the document is open before a @code willSave@endcode
 * notification is sent since clients can't change the content of a file without
 * ownership transferal.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.synchronization.willSave@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports @code textDocument/willSave@endcode
 * notifications.
 *
 * Server Capability:
 *  - property name (optional): @code textDocumentSync.willSave@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the server is interested in @code textDocument/willSave@endcode
 * notifications.
 *
 * Registration Options: @code TextDocumentRegistrationOptions@endcode
 */
using WillSaveTextDocument = glz::rpc::method<
        "textDocument/willSave",
        WillSaveTextDocumentParams,
        _Void
>;

/** WillSaveWaitUntilTextDocument Request
 *
 * The document will save request is sent from the client to the server before
 * the document is actually saved. The request can return an array of TextEdits
 * which will be applied to the text document before it is saved. Please note
 * that clients might drop results if computing the text edits took too long or
 * if a server constantly fails on this request. This is done to keep the save
 * fast and reliable. If a server has registered for open / close events clients
 * should ensure that the document is open before a @code willSaveWaitUntil@endcode
 * notification is sent since clients can't change the content of a file without
 * ownership transferal.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.synchronization.willSaveWaitUntil@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports @code textDocument/willSaveWaitUntil@endcode
 * notifications.
 *
 * Server Capability:
 *  - property name (optional): @code textDocumentSync.willSaveWaitUntil@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the server is interested in @code textDocument/willSaveWaitUntil@endcode
 * notifications.
 *
 * Registration Options: @code TextDocumentRegistrationOptions@endcode
 */
using WillSaveWaitUntilTextDocumentRequest = glz::rpc::method<
        "textDocument/willSaveWaitUntil",
        WillSaveTextDocumentParams,
        std::variant<null, std::vector<TextEdit>>
>;

/** DidSaveTextDocument Notification
 *
 * The document save notification is sent from the client to the server when the
 * document was saved in the client.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.synchronization.didSave@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports @code textDocument/didSave@endcode
 * notifications.
 *
 * Server Capability:
 *  - property name (optional): @code textDocumentSync.save@endcode
 *  - property type: @code boolean | SaveOptions@endcode
 *
 * The capability indicates that the server is interested in @code textDocument/didSave@endcode
 * notifications.
 *
 * Registration Options: @code TextDocumentSaveRegistrationOptions@endcode
 */
using DidSaveTextDocumentNotification = glz::rpc::method<
        "textDocument/didSave",
        DidSaveTextDocumentParams,
        _Void
>;

/** DidCloseTextDocument Notification
 *
 * The document close notification is sent from the client to the server when the
 * document got closed in the client. The document's master now exists where the
 * document's Uri points to (e.g. if the document's Uri is a file Uri the master
 * now exists on disk). As with the open notification the close notification is
 * about managing the document's content. Receiving a close notification doesn't
 * mean that the document was open in an editor before. A close notification
 * requires a previous open notification to be sent. Note that a server's ability
 * to fulfill requests is independent of whether a text document is open or closed.
 *
 * Client Capability:
 *  - property path (optional): @code textDocument.synchronization.dynamicRegistration@endcode
 *  - property type: @code boolean@endcode
 *
 * Server Capability:
 *  - property path (optional): @code textDocumentSync@endcode
 *  - property type: @code TextDocumentSyncKind | TextDocumentSyncOptions@endcode.
 *
 * Registration Options: @code TextDocumentRegistrationOptions@endcode
 */
using DidCloseTextDocumentNotification = glz::rpc::method<
        "textDocument/didClose",
        DidCloseTextDocumentParams,
        _Void
>;

/** DidOpenNotebookDocument Notification
 *
 * The open notification is sent from the client to the server when a notebook
 * document is opened. It is only sent by a client if the server requested the
 * synchronization mode @code notebook@endcode in its @code notebookDocumentSync@endcode
 * capability.
 */
using DidOpenNotebookDocumentNotification = glz::rpc::method<
        "notebookDocument/didOpen",
        DidOpenNotebookDocumentParams,
        _Void
>;

/** DidChangeNotebookDocument Notification
 *
 * The change notification is sent from the client to the server when a notebook
 * document changes. It is only sent by a client if the server requested the
 * synchronization mode @code notebook@endcode in its @code notebookDocumentSync@endcode
 * capability.
 */
using DidChangeNotebookDocumentNotification = glz::rpc::method<
        "notebookDocument/didChange",
        DidChangeNotebookDocumentParams,
        _Void
>;

/** DidSaveNotebookDocument Notification
 *
 * The save notification is sent from the client to the server when a notebook
 * document is saved. It is only sent by a client if the server requested the
 * synchronization mode @code notebook@endcode in its @code notebookDocumentSync@endcode
 * capability.
 */
using DidSaveNotebookDocumentNotification = glz::rpc::method<
        "notebookDocument/didSave",
        DidSaveNotebookDocumentParams,
        _Void
>;

/** DidCloseNotebookDocument Notification
 *
 * The close notification is sent from the client to the server when a notebook
 * document is closed. It is only sent by a client if the server requested the
 * synchronization mode @code notebook@endcode in its @code notebookDocumentSync@endcode
 * capability.
 */
using DidCloseNotebookDocumentNotification = glz::rpc::method<
        "notebookDocument/didClose",
        DidCloseNotebookDocumentParams,
        _Void
>;

/** Goto Declaration Request
 *
 * The go to declaration request is sent from the client to the server to resolve
 * the declaration location of a symbol at a given text document position.
 *
 * The result type @code LocationLink[]@endcode got introduced with version 3.14.0
 * and depends on the corresponding client capability
 * @code textDocument.declaration.linkSupport@endcode.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.declaration@endcode
 *  - property type: @code DeclarationClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code declarationProvider@endcode
 *  - property type: @code boolean | DeclarationOptions | DeclarationRegistrationOptions@endcode
 *
 * Registration Options: @code DeclarationRegistrationOptions@endcode
 */
using GotoDeclarationRequest = glz::rpc::method<
        "textDocument/declaration",
        DeclarationParams,
        std::variant<null, Location, std::vector<Location>, std::vector<LocationLink>>
>;

/** Goto Definition Request
 *
 * The go to definition request is sent from the client to the server to resolve
 * the definition location of a symbol at a given text document position.
 *
 * The result type @code LocationLink[]@endcode got introduced with version 3.14.0
 * and depends on the corresponding client capability
 * @code textDocument.definition.linkSupport@endcode.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.definition@endcode
 *  - property type: @code DefinitionClientCapabilities@endcode defined as follows:
 *
 * Server Capability:
 *  - property name (optional): @code definitionProvider@endcode
 *  - property type: @code boolean | DefinitionOptions@endcode
 *
 * Registration Options: @code DefinitionRegistrationOptions@endcode
 */
using GotoDefinitionRequest = glz::rpc::method<
        "textDocument/definition",
        DefinitionParams,
        std::variant<null, Location, std::vector<Location>, std::vector<LocationLink>>
>;

/** Goto Type Definition Request
 *
 * @version 3.6.0
 *
 * The go to type definition request is sent from the client to the server to
 * resolve the type definition location of a symbol at a given text document position.
 *
 * The result type @code LocationLink[]@endcode got introduced with version 3.14.0
 * and depends on the corresponding client capability
 * @code textDocument.typeDefinition.linkSupport@endcode.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.typeDefinition@endcode
 *  - property type: @code TypeDefinitionClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code typeDefinitionProvider@endcode
 *  - property type: @code boolean | TypeDefinitionOptions | TypeDefinitionRegistrationOptions@endcode
 *
 * Registration Options: @code TypeDefinitionRegistrationOptions@endcode
 */
using GotoTypeDefinitionRequest = glz::rpc::method<
        "textDocument/typeDefinition",
        TypeDefinitionParams,
        std::variant<null, Location, std::vector<Location>, std::vector<LocationLink>>
>;

/** Goto Implementation Request
 *
 * @version 3.6.0
 *
 * The go to implementation request is sent from the client to the server to resolve
 * the implementation location of a symbol at a given text document position.
 *
 * The result type @code LocationLink[]@endcode got introduced with version 3.14.0
 * and depends on the corresponding client capability
 * @code textDocument.implementation.linkSupport@endcode.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.implementation@endcode
 *  - property type: @code ImplementationClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code implementationProvider@endcode
 *  - property type: @code boolean | ImplementationOptions | ImplementationRegistrationOptions@endcode
 *
 * Registration Options: @code ImplementationRegistrationOptions@endcode
 */
using GotoImplementationRequest = glz::rpc::method<
        "textDocument/implementation",
        ImplementationParams,
        std::variant<null, Location, std::vector<Location>, std::vector<LocationLink>>
>;

/** Find References Request
 *
 * The references request is sent from the client to the server to resolve project-wide
 * references for the symbol denoted by the given text document position.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.references@endcode
 *  - property type: @code ReferenceClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code referencesProvider@endcode
 *  - property type: @code boolean | ReferenceOptions@endcode
 *
 * Registration Options: @code ReferenceRegistrationOptions@endcode
 */
using FindReferencesRequest = glz::rpc::method<
        "textDocument/references",
        ReferenceParams,
        std::variant<null, std::vector<Location>>
>;

/** Prepare Call Hierarchy Request
 *
 * @version 3.16.0
 *
 * The call hierarchy request is sent from the client to the server to return a
 * call hierarchy for the language element of given text document positions.
 * The call hierarchy requests are executed in two steps:
 *  - first a call hierarchy item is resolved for the given text document position
 *  - for a call hierarchy item the incoming or outgoing call hierarchy items are
 *    resolved.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.callHierarchy@endcode
 *  - property type: @code CallHierarchyClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code callHierarchyProvider@endcode
 *  - property type: @code boolean | CallHierarchyOptions | CallHierarchyRegistrationOptions@endcode
 *
 * Registration Options: @code CallHierarchyRegistrationOptions@endcode
 */
using PrepareCallHierarchyRequest = glz::rpc::method<
        "textDocument/prepareCallHierarchy",
        CallHierarchyPrepareParams,
        std::variant<null, std::vector<CallHierarchyItem>>
>;

/** Call Hierarchy Incoming Calls
 *
 * @version 3.16.0
 *
 * The request is sent from the client to the server to resolve incoming calls for
 * a given call hierarchy item. The request doesn't define its own client and server
 * capabilities. It is only issued if a server registers for the
 * @code textDocument/prepareCallHierarchy@endcode request.
 */
using CallHierarchyIncomingCalls = glz::rpc::method<
        "callHierarchy/incomingCalls",
        CallHierarchyIncomingCallsParams,
        std::variant<null, std::vector<CallHierarchyIncomingCall>>
>;

/** Call Hierarchy Outgoing Calls
 *
 * @version 3.16.0
 *
 * The request is sent from the client to the server to resolve outgoing calls for
 * a given call hierarchy item. The request doesn't define its own client and server
 * capabilities. It is only issued if a server registers for the
 * @code textDocument/prepareCallHierarchy@endcode request.
 */
using CallHierarchyOutgoingCalls = glz::rpc::method<
        "callHierarchy/outgoingCalls",
        CallHierarchyOutgoingCallsParams,
        std::variant<null, std::vector<CallHierarchyOutgoingCall>>
>;

/** Prepare Type Hierarchy Request
 *
 * @version 3.17.0
 *
 * The type hierarchy request is sent from the client to the server to return a
 * type hierarchy for the language element of given text document positions. Will
 * return @code null@endcode if the server couldn't infer a valid type from the
 * position. The type hierarchy requests are executed in two steps:
 *  - first a type hierarchy item is prepared for the given text document position.
 *  - for a type hierarchy item the supertype or subtype type hierarchy items are resolved.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.typeHierarchy@endcode
 *  - property type: @code TypeHierarchyClientCapabilities@endcode
 *
 * Server Capabilities:
 *  - property name (optional): @code typeHierarchyProvider@endcode
 *  - property type: @code boolean | TypeHierarchyOptions | TypeHierarchyRegistrationOptions@endcode
 *
 * Registration Options: @code TypeHierarchyRegistrationOptions@endcode
 */
using PrepareTypeHierarchyRequest = glz::rpc::method<
        "textDocument/prepareTypeHierarchy",
        TypeHierarchyPrepareParams,
        std::variant<null, std::vector<TypeHierarchyItem>>
>;

/** Type Hierarchy Supertypes
 *
 * @version 3.17.0
 *
 * The request is sent from the client to the server to resolve the supertypes for
 * a given type hierarchy item. Will return @code null@endcode if the server
 * couldn't infer a valid type from @code item@endcode in the params. The request
 * doesn't define its own client and server capabilities. It is only issued if a
 * server registers for the textDocument/prepareTypeHierarchy request.
 */
using TypeHierarchySupertypes = glz::rpc::method<
        "typeHierarchy/supertypes",
        TypeHierarchySupertypesParams,
        std::variant<null, std::vector<TypeHierarchyItem>>
>;

/** Type Hierarchy Subtypes
 *
 * @version 3.17.0
 *
 * The request is sent from the client to the server to resolve the subtypes for
 * a given type hierarchy item. Will return @code null@endcode if the server
 * couldn't infer a valid type from @code item@endcode in the params. The request
 * doesn't define its own client and server capabilities. It is only issued if a
 * server registers for the textDocument/prepareTypeHierarchy request.
 */
using TypeHierarchySubtypes = glz::rpc::method<
        "typeHierarchy/subtypes",
        TypeHierarchySubtypesParams,
        std::variant<null, std::vector<TypeHierarchyItem>>
>;

/** Document Highlights Request
 *
 * The document highlight request is sent from the client to the server to resolve
 * document highlights for a given text document position. For programming languages
 * this usually highlights all references to the symbol scoped to this file. However,
 * we kept 'textDocument/documentHighlight' and 'textDocument/references' separate
 * requests since the first one is allowed to be more fuzzy. Symbol matches usually
 * have a @code DocumentHighlightKind@endcode of @code Read@endcode or @code Write@endcode
 * whereas fuzzy or textual matches use @code Text@endcode as the kind.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.documentHighlight@endcode
 *  - property type: @code DocumentHighlightClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code documentHighlightProvider@endcode
 *  - property type: @code boolean | DocumentHighlightOptions where DocumentHighlightOptions@endcode
 *
 * Registration Options: @code DocumentHighlightRegistrationOptions@endcode
 */
using DocumentHighlightsRequest = glz::rpc::method<
        "textDocument/documentHighlight",
        DocumentHighlightParams,
        std::variant<null, std::vector<DocumentHighlight>>
>;

/** Document Link Request
 *
 * The document links request is sent from the client to the server to request the
 * location of links in a document.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.documentLink@endcode
 *  - property type: @code DocumentLinkClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code documentLinkProvider@endcode
 *  - property type: @code DocumentLinkOptions@endcode
 *
 * Registration Options: @code DocumentLinkRegistrationOptions@endcode
 */
using DocumentLinkRequest = glz::rpc::method<
        "textDocument/documentLink",
        DocumentLinkParams,
        std::variant<null, std::vector<DocumentLink>>
>;

/** Document Link Resolve Request
 *
 * The document link resolve request is sent from the client to the server to resolve
 * the target of a given document link.
 */
using DocumentLinkResolveRequest = glz::rpc::method<
        "documentLink/resolve",
        DocumentLink,
        std::variant<null, std::vector<DocumentLink>>
>;

/** Hover Request
 *
 * The hover request is sent from the client to the server to request hover
 * information at a given text document position.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.hover@endcode
 *  - property type: @code HoverClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code hoverProvider@endcode
 *  - property type: @code boolean | HoverOptions@endcode
 *
 * Registration Options: @code HoverRegistrationOptions@endcode
 */
using HoverRequest = glz::rpc::method<
        "textDocument/hover",
        HoverParams,
        std::variant<null, Hover>
>;

/** Code Lens Request
 *
 * The code lens request is sent from the client to the server to compute code
 * lenses for a given text document.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.codeLens@endcode
 *  - property type: @code CodeLensClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code codeLensProvider@endcode
 *  - property type: @code CodeLensOptions@endcode
 *
 * Registration Options: @code CodeLensRegistrationOptions@endcode
 */
using CodeLensRequest = glz::rpc::method<
        "textDocument/codeLens",
        CodeLensParams,
        std::variant<null, std::vector<CodeLens>>
>;

/** Code Lens Resolve Request
 *
 * The code lens resolve request is sent from the client to the server to resolve
 * the command for a given code lens item.
 */
using CodeLensResolveRequest = glz::rpc::method<
        "codeLens/resolve",
        CodeLens,
        CodeLens
>;

/** Code Lens Refresh Request
 *
 * @version 3.16.0
 *
 * The @code workspace/codeLens/refresh@endcode request is sent from the server
 * to the client. Servers can use it to ask clients to refresh the code lenses
 * currently shown in editors. As a result the client should ask the server to
 * recompute the code lenses for these editors. This is useful if a server detects
 * a configuration change which requires a re-calculation of all code lenses. Note
 * that the client still has the freedom to delay the re-calculation of the code
 * lenses if for example an editor is currently not visible.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.codeLens@endcode
 *  - property type: @code CodeLensWorkspaceClientCapabilities@endcode
 */
using CodeLensRefreshRequest = glz::rpc::method<
        "workspace/codeLens/refresh",
        _Void,
        _Void
>;

/** Folding Range Request
 *
 * @version 3.10.0
 *
 * The folding range request is sent from the client to the server to return all
 * folding ranges found in a given text document.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.foldingRange@endcode
 *  - property type: @code FoldingRangeClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code foldingRangeProvider@endcode
 *  - property type: @code boolean | FoldingRangeOptions | FoldingRangeRegistrationOptions@endcode
 *
 * Registration Options: @code FoldingRangeRegistrationOptions@endcode
 */
using FoldingRangeRequest = glz::rpc::method<
        "textDocument/foldingRange",
        FoldingRangeParams,
        std::variant<null, std::vector<FoldingRange>>
>;

/** Folding Range Refresh Request
 *
 * @version 3.18.0
 *
 * The @code workspace/foldingRange/refresh@endcode request is sent from the server
 * to the client. Servers can use it to ask clients to refresh the folding ranges
 * currently shown in editors. As a result, the client should ask the server to
 * recompute the folding ranges for these editors. This is useful if a server
 * detects a configuration change which requires a re-calculation of all folding
 * ranges. Note that the client still has the freedom to delay the re-calculation
 * of the folding ranges if, for example, an editor is currently not visible.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.foldingRange@endcode
 *  - property type: @code FoldingRangeWorkspaceClientCapabilities@endcode
 */
using FoldingRangeRefreshRequest = glz::rpc::method<
        "workspace/foldingRange/refresh",
        _Void,
        _Void
>;

/** Selection Range Request
 *
 * @version 3.15.0
 *
 * The selection range request is sent from the client to the server to return
 * suggested selection ranges at an array of given positions. A selection range
 * is a range around the cursor position which the user might be interested in selecting.
 *
 * A selection range in the return array is for the position in the provided parameters
 * at the same index. Therefore positions[i] must be contained in result[i].range.
 * To allow for results where some positions have selection ranges and others do not,
 * result[i].range is allowed to be the empty range at positions[i].
 *
 * Typically, but not necessary, selection ranges correspond to the nodes of the syntax tree.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.selectionRange@endcode
 *  - property type: @code SelectionRangeClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code selectionRangeProvider@endcode
 *  - property type: @code boolean | SelectionRangeOptions | SelectionRangeRegistrationOptions@endcode
 *
 * Registration Options: @code SelectionRangeRegistrationOptions@endcode
 */
using SelectionRangeRequest = glz::rpc::method<
        "textDocument/selectionRange",
        SelectionRangeParams,
        std::variant<null, std::vector<SelectionRange>>
>;

/** Document Symbols Request
 *
 * The document symbol request is sent from the client to the server. The returned
 * result is either:
 *  - @code SymbolInformation[]@endcode which is a flat list of all symbols found
 *    in a given text document. Then neither the symbol's location range nor the
 *    symbol's container name should be used to infer a hierarchy.
 *  - @code DocumentSymbol[]@endcode which is a hierarchy of symbols found in a
 *    given text document.
 *
 * Servers should whenever possible return @code DocumentSymbol@endcode since it
 * is the richer data structure.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.documentSymbol@endcode
 *  - property type: @code DocumentSymbolClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code documentSymbolProvider@endcode
 *  - property type: @code boolean | DocumentSymbolOptions where DocumentSymbolOptions@endcode
 *
 * Registration Options: @code DocumentSymbolRegistrationOptions@endcode
 */
using DocumentSymbolsRequest = glz::rpc::method<
        "textDocument/documentSymbol",
        DocumentSymbolParams,
        std::variant<null, std::vector<DocumentSymbol>, std::vector<SymbolInformation>>
>;

/** Semantic Tokens
 *
 * @version 3.16.0
 *
 * The request is sent from the client to the server to resolve semantic tokens
 * for a given file. Semantic tokens are used to add additional color information
 * to a file that depends on language specific symbol information. A semantic token
 * request usually produces a large result. The protocol therefore supports encoding
 * tokens with numbers. In addition optional support for deltas is available.
 *
 * General Concepts
 *
 * Tokens are represented using one token type combined with n token modifiers.
 * A token type is something like @code class@endcode or @code function@endcode
 * and token modifiers are like @code static@endcode or @code async@endcode.
 * The protocol defines a set of token types and modifiers but clients are allowed
 * to extend these and announce the values they support in the corresponding
 * client capability.
 *
 * The protocol defines an additional token format capability to allow future
 * extensions of the format. The only format that is currently specified is
 * @code relative@endcode expressing that the tokens are described using relative
 * positions.
 *
 * Integer Encoding for Tokens
 *
 * On the capability level types and modifiers are defined using strings. However
 * the real encoding happens using numbers. The server therefore needs to let the
 * client know which numbers it is using for which types and modifiers. They do
 * so using a legend.
 *
 * Token types are looked up by index, so a @code tokenType@endcode value of
 * @code 1@endcode means @code tokenTypes[1]@endcode. Since a token type can have
 * n modifiers, multiple token modifiers can be set by using bit flags, so a
 * @code tokenModifier@endcode value of 3 is first viewed as binary
 * @code 0b00000011@endcode, which means @code [tokenModifiers[0], tokenModifiers[1]]@endcode
 * because bits 0 and 1 are set.
 *
 * There are different ways how the position of a token can be expressed in a file.
 * Absolute positions or relative positions. The protocol for the token format
 * @code relative@endcode uses relative positions, because most tokens remain stable
 * relative to each other when edits are made in a file. This simplifies the
 * computation of a delta if a server supports it. So each token is represented
 * using 5 integers. A specific token @code i@endcode in the file consists of
 * the following array indices:
 *  - at index @code 5*i@endcode - @code deltaLine@endcode: token line number,
 *    relative to the start of the previous token
 *  - at index @code 5*i+1@endcode - @code deltaStart@endcode: token start character,
 *    relative to the start of the previous token (relative to 0 or the previous
 *    token's start if they are on the same line)
 *  - at index @code 5*i+2@endcode - @code length@endcode: the length of the token.
 *  - at index @code 5*i+3@endcode - @code tokenType@endcode: will be looked up in
 *    @code SemanticTokensLegend.tokenTypes@endcode. We currently ask that
 *    @code tokenType@endcode < 65536.
 *  - at index @code 5*i+4@endcode - @code tokenModifiers@endcode: each set bit
 *    will be looked up in @code SemanticTokensLegend.tokenModifiers@endcode
 *
 * The @code deltaStart@endcode and the @code length@endcode values must be encoded
 * using the encoding the client and server agrees on during the @code initialize@endcode
 * request (see also TextDocuments). Whether a token can span multiple lines is
 * defined by the client capability @code multilineTokenSupport@endcode. If
 * multiline tokens are not supported and a tokens length takes it past the end
 * of the line, it should be treated as if the token ends at the end of the line
 * and will not wrap onto the next line.
 *
 * The client capability @code overlappingTokenSupport@endcode defines whether
 * tokens can overlap each other.
 *
 * Client Capability:
 *
 * The following client capabilities are defined for semantic token requests sent from the client to the server:
 *  - property name (optional): @code textDocument.semanticTokens@endcode
 *  - property type: @code SemanticTokensClientCapabilities@endcode
 *
 * Server Capability:
 *
 * The following server capabilities are defined for semantic tokens:
 *  - property name (optional): @code semanticTokensProvider@endcode
 *  - property type: @code SemanticTokensOptions | SemanticTokensRegistrationOptions@endcode
 *
 * Registration Options: @code SemanticTokensRegistrationOptions@endcode
 *
 * Since the registration option handles range, full and delta requests the method
 * used to register for semantic tokens requests is @code textDocument/semanticTokens@endcode.
 */
using SemanticTokensFullRequest = glz::rpc::method<
        "textDocument/semanticTokens/full",
        SemanticTokensParams,
        std::variant<null, SemanticTokens>
>;

using SemanticTokensDeltaFullRequest = glz::rpc::method<
        "textDocument/semanticTokens/full/delta",
        SemanticTokensDeltaParams,
        std::variant<null, SemanticTokens, SemanticTokensDelta>
>;

/** Requesting semantic tokens for a range
 *
 * There are two uses cases where it can be beneficial to only compute semantic
 * tokens for a visible range:
 *  - for faster rendering of the tokens in the user interface when a user opens
 *    a file. In this use cases servers should also implement the
 *    @code textDocument/semanticTokens/full@endcode request as well to allow for
 *    flicker free scrolling and semantic coloring of a minimap.
 *  - if computing semantic tokens for a full document is too expensive servers
 *    can only provide a range call. In this case the client might not render a
 *    minimap correctly or might even decide to not show any semantic tokens at all.
 *
 * A server is allowed to compute the semantic tokens for a broader range than
 * requested by the client. However if the server does the semantic tokens for
 * the broader range must be complete and correct. If a token at the beginning or
 * end only partially overlaps with the requested range the server should include
 * those tokens in the response.
 */
using SemanticTokensRangeRequest = glz::rpc::method<
        "textDocument/semanticTokens/range",
        SemanticTokensRangeParams,
        std::variant<null, SemanticTokens>
>;

/** Requesting a refresh of all semantic tokens
 *
 * The @code workspace/semanticTokens/refresh@endcode request is sent from the
 * server to the client. Servers can use it to ask clients to refresh the editors
 * for which this server provides semantic tokens. As a result the client should
 * ask the server to recompute the semantic tokens for these editors. This is
 * useful if a server detects a project wide configuration change which requires
 * a re-calculation of all semantic tokens. Note that the client still has the
 * freedom to delay the re-calculation of the semantic tokens if for example an
 * editor is currently not visible.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.semanticTokens@endcode
 *  - property type: @code SemanticTokensWorkspaceClientCapabilities@endcode
 */
using SemanticTokensRefreshRequest = glz::rpc::method<
        "textDocument/semanticTokens/refresh",
        _Void,
        _Void
>;

/** Inlay Hint Request
 *
 * @version 3.17.0
 *
 * The inlay hints request is sent from the client to the server to compute inlay
 * hints for a given [text document, range] tuple that may be rendered in the
 * editor in place with other text.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.inlayHint@endcode
 *  - property type: @code InlayHintClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code inlayHintProvider@endcode
 *  - property type: @code InlayHintOptions@endcode
 *
 * Registration Options: @code InlayHintRegistrationOptions@endcode
 */
using InlayHintRequest = glz::rpc::method<
        "textDocument/inlayHint",
        InlayHintParams,
        std::variant<null, std::vector<InlayHint>>
>;

/** Inlay Hint Resolve Request
 *
 * @version 3.17.0
 *
 * The request is sent from the client to the server to resolve additional
 * information for a given inlay hint. This is usually used to compute the
 * @code tooltip@endcode, @code location@endcode or @code command@endcode properties
 * of an inlay hint's label part to avoid its unnecessary computation during the
 * @code textDocument/inlayHint@endcode request.
 *
 * Consider the client announcing the @code label.location@endcode property as a
 * property that can be resolved lazily using the client capability
 * @code{.ts}
 * textDocument.inlayHint.resolveSupport = { properties: ['label.location'] };
 * @endcode
 *
 * then an inlay hint with a label part without a location needs to be resolved
 * using the @code inlayHint/resolve@endcode request before it can be used.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.inlayHint.resolveSupport@endcode
 *  - property type: @code { properties: string[]; }@endcode
 */
using InlayHintResolveRequest = glz::rpc::method<
        "inlayHint/resolve",
        InlayHint,
        InlayHint
>;

/** Inlay Hint Refresh Request
 *
 * @version 3.17.0
 *
 * The @code workspace/inlayHint/refresh@endcode request is sent from the server
 * to the client. Servers can use it to ask clients to refresh the inlay hints
 * currently shown in editors. As a result the client should ask the server to
 * recompute the inlay hints for these editors. This is useful if a server detects
 * a configuration change which requires a re-calculation of all inlay hints.
 * Note that the client still has the freedom to delay the re-calculation of the
 * inlay hints if for example an editor is currently not visible.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.inlayHint@endcode
 *  - property type: @code InlayHintWorkspaceClientCapabilities@endcode
 */
using InlayHintRefreshRequest = glz::rpc::method<
        "workspace/inlayHint/refresh",
        _Void,
        _Void
>;

/** Inline Value Request
 *
 * @version 3.17.0
 *
 * The inline value request is sent from the client to the server to compute inline
 * values for a given text document that may be rendered in the editor at the end
 * of lines.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.inlineValue@endcode
 *  - property type: @code InlineValueClientCapabilities@endcode
 *
 * Registration Options: @code InlineValueRegistrationOptions@endcode
 */
using InlineValueRequest = glz::rpc::method<
        "textDocument/inlineValue",
        InlineValueParams,
        std::variant<null, std::vector<InlineValue>>
>;

/** Inline Value Refresh Request
 *
 * @version 3.17.0
 *
 * The @code workspace/inlineValue/refresh@endcode request is sent from the server
 * to the client. Servers can use it to ask clients to refresh the inline values
 * currently shown in editors. As a result the client should ask the server to
 * recompute the inline values for these editors. This is useful if a server
 * detects a configuration change which requires a re-calculation of all inline
 * values. Note that the client still has the freedom to delay the re-calculation
 * of the inline values if for example an editor is currently not visible.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.inlineValue@endcode
 *  - property type: @code InlineValueWorkspaceClientCapabilities@endcode
 */
using InlineValueRefreshRequest = glz::rpc::method<
        "workspace/inlineValue/refresh",
        _Void,
        _Void
>;

/** Monikers
 *
 * @version 3.16.0
 *
 * Language Server Index Format (LSIF) introduced the concept of symbol monikers
 * to help associate symbols across different indexes. This request adds capability
 * for LSP server implementations to provide the same symbol moniker information
 * given a text document position. Clients can utilize this method to get the
 * moniker at the current location in a file user is editing and do further code
 * navigation queries in other services that rely on LSIF indexes and link symbols
 * together.
 *
 * The @code textDocument/moniker@endcode request is sent from the client to the
 * server to get the symbol monikers for a given text document position. An array
 * of Moniker types is returned as response to indicate possible monikers at the
 * given location. If no monikers can be calculated, an empty array or @code null@endcode
 * should be returned.
 *
 * Client Capabilities:
 *  - property name (optional): @code textDocument.moniker@endcode
 *  - property type: @code MonikerClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code monikerProvider@endcode
 *  - property type: @code boolean | MonikerOptions | MonikerRegistrationOptions@endcode
 *
 * Registration Options: @code MonikerRegistrationOptions@endcode
 */
using Monikers = glz::rpc::method<
        "textDocument/moniker",
        MonikerParams,
        std::variant<null, std::vector<Moniker>>
>;

/** Completion Request
 *
 * The Completion request is sent from the client to the server to compute completion
 * items at a given cursor position. Completion items are presented in the IntelliSense
 * user interface. If computing full completion items is expensive, servers can
 * additionally provide a handler for the completion item resolve request
 * ('completionItem/resolve'). This request is sent when a completion item is
 * selected in the user interface. A typical use case is for example: the
 * @code textDocument/completion@endcode request doesn't fill in the
 * @code documentation@endcode property for returned completion items since it is
 * expensive to compute. When the item is selected in the user interface then a
 * 'completionItem/resolve' request is sent with the selected completion item as
 * a parameter. The returned completion item should have the documentation property
 * filled in. By default the request can only delay the computation of the
 * @code detail@endcode and @code documentation@endcode properties. Since 3.16.0
 * the client can signal that it can resolve more properties lazily. This is done
 * using the @code completionItem#resolveSupport@endcode client capability which
 * lists all properties that can be filled in during a 'completionItem/resolve'
 * request. All other properties (usually @code sortText@endcode, @code filterText@endcode,
 * @code insertText@endcode and @code textEdit@endcode) must be provided in the
 * @code textDocument/completion@endcode response and must not be changed during
 * resolve.
 *
 * The language server protocol uses the following model around completions:
 *  - to achieve consistency across languages and to honor different clients usually
 *    the client is responsible for filtering and sorting. This has also the advantage
 *    that client can experiment with different filter and sorting models. However
 *    servers can enforce different behavior by setting a @code filterText@endcode /
 *    @code sortText@endcode
 *  - for speed clients should be able to filter an already received completion list
 *    if the user continues typing. Servers can opt out of this using a
 *    @code CompletionList@endcode and mark it as @code isIncomplete@endcode.
 *
 * A completion item provides additional means to influence filtering and sorting.
 * They are expressed by either creating a @code CompletionItem@endcode with a
 * @code insertText@endcode or with a @code textEdit@endcode. The two modes differ
 * as follows:
 *  - Completion item provides an insertText / label without a text edit: in the
 *    model the client should filter against what the user has already typed using
 *    the word boundary rules of the language (e.g. resolving the word under the
 *    cursor position). The reason for this mode is that it makes it extremely easy
 *    for a server to implement a basic completion list and get it filtered on the
 *    client.
 *  - Completion Item with text edits: in this mode the server tells the client that
 *    it actually knows what it is doing. If you create a completion item with a text
 *    edit at the current cursor position no word guessing takes place and no automatic
 *    filtering (like with an @code insertText@endcode) should happen. This mode
 *    can be combined with a sort text and filter text to customize two things.
 *    If the text edit is a replace edit then the range denotes the word used for
 *    filtering. If the replace changes the text it most likely makes sense to
 *    specify a filter text to be used.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.completion@endcode
 *  - property type: @code CompletionClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code completionProvider@endcode
 *  - property type: @code CompletionOptions@endcode
 *
 * Registration Options: @code CompletionRegistrationOptions@endcode
 */
using CompletionRequest = glz::rpc::method<
        "textDocument/completion",
        CompletionParams,
        std::variant<null, std::vector<CompletionItem>, CompletionList>
>;

/** Completion Item Resolve Request
 *
 * The request is sent from the client to the server to resolve additional
 * information for a given completion item.
 */
using CompletionItemResolveRequest = glz::rpc::method<
        "completionItem/resolve",
        CompletionItem,
        CompletionItem
>;

/** PublishDiagnostics Notification
 *
 * Diagnostics notifications are sent from the server to the client to signal
 * results of validation runs.
 *
 * Diagnostics are “owned” by the server so it is the server's responsibility to
 * clear them if necessary. The following rule is used for VS Code servers that
 * generate diagnostics:
 *  - if a language is single file only (for example HTML) then diagnostics are
 *    cleared by the server when the file is closed. Please note that open / close
 *    events don't necessarily reflect what the user sees in the user interface.
 *    These events are ownership events. So with the current version of the
 *    specification it is possible that problems are not cleared although the
 *    file is not visible in the user interface since the client has not closed
 *    the file yet.
 *  - if a language has a project system (for example C#) diagnostics are not
 *    cleared when a file closes. When a project is opened all diagnostics for all
 *    files are recomputed (or read from a cache).
 *
 * When a file changes it is the server's responsibility to re-compute diagnostics
 * and push them to the client. If the computed set is empty it has to push the
 * empty array to clear former diagnostics. Newly pushed diagnostics always replace
 * previously pushed diagnostics. There is no merging that happens on the client
 * side.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.publishDiagnostics@endcode
 *  - property type: @code PublishDiagnosticsClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code diagnosticProvider@endcode
 *  - property type: @code DiagnosticOptions@endcode
 *
 * Registration Options: @code DiagnosticRegistrationOptions@endcode
 */
using PublishDiagnosticsNotification = glz::rpc::method<
        "textDocument/publishDiagnostics",
        PublishDiagnosticsParams,
        _Void
>;

/** Pull Diagnostics
 *
 * Diagnostics are currently published by the server to the client using a notification.
 * This model has the advantage that for workspace wide diagnostics the server has
 * the freedom to compute them at a server preferred point in time. On the other
 * hand the approach has the disadvantage that the server can't prioritize the
 * computation for the file in which the user types or which are visible in the
 * editor. Inferring the client's UI state from the @code textDocument/didOpen@endcode
 * and @code textDocument/didChange@endcode notifications might lead to false
 * positives since these notifications are ownership transfer notifications.
 *
 * The specification therefore introduces the concept of diagnostic pull requests
 * to give a client more control over the documents for which diagnostics should
 * be computed and at which point in time.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.diagnostic@endcode
 *  - property type: @code DiagnosticClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code diagnosticProvider@endcode
 *  - property type: @code DiagnosticOptions@endcode
 *
 * Document Diagnostics:
 *
 * The text document diagnostic request is sent from the client to the server to
 * ask the server to compute the diagnostics for a given document. As with other
 * pull requests the server is asked to compute the diagnostics for the currently
 * synced version of the document.
 */
using DocumentDiagnostics = glz::rpc::method<
        "textDocument/diagnostic",
        DocumentDiagnosticParams,
        DocumentDiagnosticReport
>;

/** Workspace Diagnostics
 *
 * The workspace diagnostic request is sent from the client to the server to ask
 * the server to compute workspace wide diagnostics which previously were pushed
 * from the server to the client. In contrast to the document diagnostic request
 * the workspace request can be long running and is not bound to a specific workspace
 * or document state. If the client supports streaming for the workspace diagnostic
 * pull it is legal to provide a document diagnostic report multiple times for the
 * same document URI. The last one reported will win over previous reports.
 *
 * If a client receives a diagnostic report for a document in a workspace diagnostic
 * request for which the client also issues individual document diagnostic pull
 * requests the client needs to decide which diagnostics win and should be presented
 * In general:
 *  - diagnostics for a higher document version should win over those from a lower
 *    document version (e.g. note that document versions are steadily increasing)
 *  - diagnostics from a document pull should win over diagnostics from a workspace
 *    pull.
 */
using WorkspaceDiagnostics = glz::rpc::method<
        "workspace/diagnostic",
        WorkspaceDiagnosticParams,
        WorkspaceDiagnosticReport
>;

/** Diagnostics Refresh
 *
 * The @code workspace/diagnostic/refresh@endcode request is sent from the server
 * to the client. Servers can use it to ask clients to refresh all needed document
 * and workspace diagnostics. This is useful if a server detects a project wide
 * configuration change which requires a re-calculation of all diagnostics.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.diagnostics@endcode
 *  - property type: @code DiagnosticWorkspaceClientCapabilities@endcode
 */
using DiagnosticsRefresh = glz::rpc::method<
        "workspace/diagnostic/refresh",
        _Void,
        _Void
>;

/** Signature Help Request
 *
 * The signature help request is sent from the client to the server to request
 * signature information at a given cursor position.
 *
 * Client Capabilities:
 *  - property name (optional): @code textDocument.signatureHelp@endcode
 *  - property type: @code SignatureHelpClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code signatureHelpProvider@endcode
 *  - property type: @code SignatureHelpOptions@endcode
 *
 * Registration Options: @code SignatureHelpRegistrationOptions@endcode
 */
using SignatureHelpRequest = glz::rpc::method<
        "textDocument/signatureHelp",
        SignatureHelpParams,
        std::variant<null, SignatureHelp>
>;

/** Code Action Request
 *
 * The code action request is sent from the client to the server to compute commands
 * for a given text document and range. These commands are typically code fixes
 * to either fix problems or to beautify/refactor code. The result of a
 * @code textDocument/codeAction@endcode request is an array of @code Command@endcode
 * literals which are typically presented in the user interface. To ensure that
 * a server is useful in many clients the commands specified in a code actions
 * should be handled by the server and not by the client (see
 * @code workspace/executeCommand@endcode and
 * @code ServerCapabilities.executeCommandProvider@endcode). If the client supports
 * providing edits with a code action then that mode should be used.
 *
 * Since version 3.16.0: a client can offer a server to delay the computation of
 * code action properties during a 'textDocument/codeAction' request:
 *
 * This is useful for cases where it is expensive to compute the value of a property
 * (for example the @code edit@endcode property). Clients signal this through the
 * @code codeAction.resolveSupport@endcode capability which lists all properties
 * a client can resolve lazily. The server capability
 * @code codeActionProvider.resolveProvider@endcode signals that a server will
 * offer a @code codeAction/resolve@endcode route. To help servers to uniquely
 * identify a code action in the resolve request, a code action literal can optional
 * carry a data property. This is also guarded by an additional client capability
 * @code codeAction.dataSupport@endcode. In general, a client should offer data
 * support if it offers resolve support. It should also be noted that servers
 * shouldn't alter existing attributes of a code action in a codeAction/resolve
 * request.
 *
 * Since version 3.8.0: support for CodeAction literals to enable the following
 * scenarios:
 *  - the ability to directly return a workspace edit from the code action request.
 *    This avoids having another server roundtrip to execute an actual code action.
 *    However server providers should be aware that if the code action is expensive
 *    to compute or the edits are huge it might still be beneficial if the result
 *    is simply a command and the actual edit is only computed when needed.
 *  - the ability to group code actions using a kind. Clients are allowed to ignore
 *    that information. However it allows them to better group code action for example
 *    into corresponding menus (e.g. all refactor code actions into a refactor menu).
 *
 * Clients need to announce their support for code action literals (e.g. literals
 * of type @code CodeAction@endcode) and code action kinds via the corresponding
 * client capability @code codeAction.codeActionLiteralSupport@endcode.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.codeAction@endcode
 *  - property type: @code CodeActionClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code codeActionProvider@endcode
 *  - property type: @code boolean | CodeActionOptions@endcode
 *
 * Registration Options: @code CodeActionRegistrationOptions@endcode
 */
using CodeActionRequest = glz::rpc::method<
        "textDocument/codeAction",
        CodeActionParams,
        std::variant<null, std::vector<std::variant<Command, CodeAction>>>
>;

/** Code Action Resolve Request
 *
 * @version 3.16.0
 *
 * The request is sent from the client to the server to resolve additional information
 * for a given code action. This is usually used to compute the @code edit@endcode
 * property of a code action to avoid its unnecessary computation during the
 * @code textDocument/codeAction@endcode request.
 *
 * Consider the client announcing the @code edit@endcode property as a property
 * that can be resolved lazily using the client capability
 * @code{.ts}
 * textDocument.codeAction.resolveSupport = { properties: ['edit'] };
 * @endcode
 *
 * then a code action
 * @code{.json}
 * {
 *     "title": "Do Foo"
 * }
 * @endcode
 *
 * needs to be resolved using the @code codeAction/resolve@endcode request before
 * it can be applied.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.codeAction.resolveSupport@endcode
 *  - property type: @code { properties: string[]; }@endcode
 */
using CodeActionResolveRequest = glz::rpc::method<
        "codeAction/resolve",
        CodeAction,
        CodeAction
>;

/** Document Color Request
 *
 * @version 3.6.0
 *
 * The document color request is sent from the client to the server to list all
 * color references found in a given text document. Along with the range, a color
 * value in RGB is returned.
 *
 * Clients can use the result to decorate color references in an editor. For example:
 *  - Color boxes showing the actual color next to the reference
 *  - Show a color picker when a color reference is edited
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.colorProvider@endcode
 *  - property type: @code DocumentColorClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code colorProvider@endcode
 *  - property type: @code boolean | DocumentColorOptions | DocumentColorRegistrationOptions@endcode
 *
 * Registration Options: @code DocumentColorRegistrationOptions@endcode
 */
using DocumentColorRequest = glz::rpc::method<
        "textDocument/documentColor",
        DocumentColorParams,
        std::vector<ColorInformation>
>;

/** Color Presentation Request
 *
 * @version 3.6.0
 *
 * The color presentation request is sent from the client to the server to obtain
 * a list of presentations for a color value at a given location. Clients can use
 * the result to:
 *  - modify a color reference.
 *  - show in a color picker and let users pick one of the presentations
 *
 * This request has no special capabilities and registration options since it is
 * sent as a resolve request for the @code textDocument/documentColor@endcode request.
 */
using ColorPresentationRequest = glz::rpc::method<
        "textDocument/colorPresentation",
        ColorPresentationParams,
        std::vector<ColorPresentation>
>;

/** Document Formatting Request
 *
 * The document formatting request is sent from the client to the server to format
 * a whole document.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.formatting@endcode
 *  - property type: @code DocumentFormattingClientCapabilities@endcode
 *
 * Server Capabilities:
 *  - property name (optional): @code documentFormattingProvider@endcode
 *  - property type: @code boolean | DocumentFormattingOptions@endcode
 *
 * Registration Options: @code DocumentFormattingRegistrationOptions@endcode
 */
using DocumentFormattingRequest = glz::rpc::method<
        "textDocument/formatting",
        DocumentFormattingParams,
        std::variant<null, std::vector<TextEdit>>
>;

/** Document Range Formatting Request
 *
 * The document range formatting request is sent from the client to the server to
 * format a given range in a document.
 *
 * @version 3.18.0 - If supported, the client may send multiple ranges at once for formatting via
 *  the @code textDocument/rangesFormatting@endcode method.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.rangeFormatting@endcode
 *  - property type: @code DocumentRangeFormattingClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code documentRangeFormattingProvider@endcode
 *  - property type: @code boolean | DocumentRangeFormattingOptions@endcode
 *
 * Registration Options: @code DocumentFormattingRegistrationOptions@endcode
 */
using DocumentRangeFormattingRequest = glz::rpc::method<
        "textDocument/rangeFormatting",
        DocumentRangeFormattingParams,
        std::variant<null, std::vector<TextEdit>>
>;

using DocumentRangesFormattingRequest = glz::rpc::method<
        "textDocument/rangesFormatting",
        DocumentRangesFormattingParams,
        std::variant<null, std::vector<TextEdit>>
>;

/** Document on Type Formatting Request
 *
 * The document on type formatting request is sent from the client to the server
 * to format parts of the document during typing.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.onTypeFormatting@endcode
 *  - property type: @code DocumentOnTypeFormattingClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code documentOnTypeFormattingProvider@endcode
 *  - property type: @code DocumentOnTypeFormattingOptions@endcode
 *
 * Registration Options: @code DocumentOnTypeFormattingRegistrationOptions@endcode
 */
using DocumentOnTypeFormattingRequest = glz::rpc::method<
        "textDocument/onTypeFormatting",
        DocumentOnTypeFormattingParams,
        std::variant<null, std::vector<TextEdit>>
>;

/** Rename Request
 *
 * The rename request is sent from the client to the server to ask the server to
 * compute a workspace change so that the client can perform a workspace-wide rename
 * of a symbol.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.rename@endcode
 *  - property type: @code RenameClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code renameProvider@endcode
 *  - property type: @code boolean | RenameOptions@endcode
 *
 * Registration Options: @code RenameRegistrationOptions@endcode
 */
using RenameRequest = glz::rpc::method<
        "textDocument/rename",
        RenameParams,
        std::variant<null, WorkspaceEdit>
>;

/** Prepare Rename Request
 *
 * @version 3.12.0
 *
 * The prepare rename request is sent from the client to the server to setup and
 * test the validity of a rename operation at a given location.
 */
using PrepareRenameRequest = glz::rpc::method<
        "textDocument/prepareRename",
        PrepareRenameParams,
        std::variant<null, Range, _PrepareRenameRequestResult1, _PrepareRenameRequestResult2>
>;

/** Linked Editing Range
 *
 * @version 3.16.0
 *
 * The linked editing request is sent from the client to the server to return for
 * a given position in a document the range of the symbol at the position and all
 * ranges that have the same content. Optionally a word pattern can be returned to
 * describe valid contents. A rename to one of the ranges can be applied to all
 * other ranges if the new content is valid. If no result-specific word pattern
 * is provided, the word pattern from the client's language configuration is used.
 *
 * Client Capabilities:
 *  - property name (optional): @code textDocument.linkedEditingRange@endcode
 *  - property type: @code LinkedEditingRangeClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code linkedEditingRangeProvider@endcode
 *  - property type: @code boolean | LinkedEditingRangeOptions | LinkedEditingRangeRegistrationOptions@endcode
 *
 * Registration Options: @code LinkedEditingRangeRegistrationOptions@endcode
 */
using LinkedEditingRange = glz::rpc::method<
        "textDocument/linkedEditingRange",
        LinkedEditingRangeParams,
        std::variant<null, LinkedEditingRanges>
>;

/** Inline Completion Request
 *
 * @version 3.18.0
 *
 * The inline completion request is sent from the client to the server to compute
 * inline completions for a given text document either explicitly by a user gesture
 * or implicitly when typing.
 *
 * Inline completion items usually complete bigger portions of text (e.g., whole
 * methods) and in contrast to completions, items can complete code that might be
 * syntactically or semantically incorrect.
 *
 * Due to this, inline completion items are usually not suited to be presented in
 * normal code completion widgets like a list of items. One possible approach can
 * be to present the information inline in the editor with lower contrast.
 *
 * When multiple inline completion items are returned, the client may decide whether
 * the user can cycle through them or if they, along with their filterText, are
 * merely for filtering if the user continues to type without yet accepting the
 * inline completion item.
 *
 * Clients may choose to send information about the user’s current completion
 * selection via context if completions are visible at the same time. In this case,
 * returned inline completions should extend the text of the provided completion.
 *
 * Client Capability:
 *  - property name (optional): @code textDocument.inlineCompletion@endcode
 *  - property type: @code InlineCompletionClientCapabilities@endcode
 *
 * Server Capability:
 *  - property name (optional): @code inlineCompletionProvider@endcode
 *  - property type: @code InlineCompletionOptions@endcode
 *
 * Registration Options: @code InlineCompletionRegistrationOptions@endcode
 */
using InlineCompletionRequest = glz::rpc::method<
        "textDocument/inlineCompletion",
        InlineCompletionParams,
        std::variant<null, std::vector<InlineCompletionItem>, InlineCompletionList>
>;

/** Workspace Symbols Request
 *
 * The workspace symbol request is sent from the client to the server to list
 * project-wide symbols matching the query string. Since 3.17.0 servers can also
 * provide a handler for @code workspaceSymbol/resolve@endcode requests. This
 * allows servers to return workspace symbols without a range for a
 * @code workspace/symbol@endcode request. Clients then need to resolve the range
 * when necessary using the @code workspaceSymbol/resolve@endcode request. Servers
 * can only use this new model if clients advertise support for it via the
 * @code workspace.symbol.resolveSupport@endcode capability.
 *
 * Client Capability:
 *  - property path (optional): @code workspace.symbol@endcode
 *  - property type: @code WorkspaceSymbolClientCapabilities@endcode
 *
 * Server Capability:
 *  - property path (optional): @code workspaceSymbolProvider@endcode
 *  - property type: @code boolean | WorkspaceSymbolOptions@endcode
 *
 * Registration Options: @code WorkspaceSymbolRegistrationOptions@endcode
 */
using WorkspaceSymbolsRequest = glz::rpc::method<
        "workspace/symbol",
        WorkspaceSymbolParams,
        std::variant<null, std::vector<SymbolInformation>, std::vector<WorkspaceSymbol>>
>;

/** Workspace Symbol Resolve Request
 *
 * The request is sent from the client to the server to resolve additional
 * information for a given workspace symbol.
 */
using WorkspaceSymbolResolveRequest = glz::rpc::method<
        "workspaceSymbol/resolve",
        WorkspaceSymbol,
        WorkspaceSymbol
>;

/** Configuration Request
 *
 * @version 3.6.0
 *
 * The @code workspace/configuration@endcode request is sent from the server to
 * the client to fetch configuration settings from the client. The request can
 * fetch several configuration settings in one roundtrip. The order of the returned
 * configuration settings correspond to the order of the passed
 * @code ConfigurationItems@endcode (e.g. the first item in the response is the
 * result for the first configuration item in the params).
 *
 * A @code ConfigurationItem@endcode consists of the configuration section to ask
 * for and an additional scope URI. The configuration section asked for is defined
 * by the server and doesn't necessarily need to correspond to the configuration
 * store used by the client. So a server might ask for a configuration
 * @code cpp.formatterOptions@endcode but the client stores the configuration in
 * an XML store layout differently. It is up to the client to do the necessary
 * conversion. If a scope URI is provided the client should return the setting
 * scoped to the provided resource. If the client for example uses EditorConfig
 * to manage its settings the configuration should be returned for the passed
 * resource URI. If the client can’t provide a configuration setting for a
 * given scope then @code null@endcode needs to be present in the returned array.
 *
 * This pull model replaces the old push model were the client signaled configuration
 * change via an event. If the server still needs to react to configuration changes
 * (since the server caches the result of @code workspace/configuration@endcode
 * requests) the server should register for an empty configuration change using
 * the following registration pattern:
 * @code{.ts}
 * connection.client.register(DidChangeConfigurationNotification.type, undefined);
 * @endcode
 *
 * Client Capability:
 *  - property path (optional): @code workspace.configuration@endcode
 *  - property type: @code boolean@endcode
 */
using ConfigurationRequest = glz::rpc::method<
        "workspace/configuration",
        ConfigurationParams,
        std::vector<LSPAny>
>;

/** DidChangeConfiguration Notification
 *
 * A notification sent from the client to the server to signal the change of
 * configuration settings.
 *
 * Client Capability:
 *  - property path (optional): @code workspace.didChangeConfiguration@endcode
 *  - property type: @code DidChangeConfigurationClientCapabilities@endcode
 */
using DidChangeConfigurationNotification = glz::rpc::method<
        "workspace/didChangeConfiguration",
        DidChangeConfigurationParams,
        _Void
>;

/** Workspace folders request
 *
 * @version 3.6.0
 *
 * Many tools support more than one root folder per workspace. Examples for this
 * are VS Code’s multi-root support, Atom’s project folder support or Sublime’s
 * project support. If a client workspace consists of multiple roots then a server
 * typically needs to know about this. The protocol up to now assumes one root
 * folder which is announced to the server by the @code rootUri@endcode property
 * of the @code InitializeParams@endcode. If the client supports workspace folders
 * and announces them via the corresponding @code workspaceFolders@endcode client
 * capability, the InitializeParams contain an additional property
 * @code workspaceFolders@endcode with the configured workspace folders when the
 * server starts.
 *
 * The @code workspace/workspaceFolders@endcode request is sent from the server
 * to the client to fetch the current open list of workspace folders. Returns
 * @code null@endcode in the response if only a single file is open in the tool.
 * Returns an empty array if a workspace is open but no folders are configured.
 *
 * Client capability:
 *  - property path (optional): @code workspace.workspaceFolders@endcode
 *  - property type: @code boolean@endcode
 *
 * Server Capability:
 *  - property path (optional): @code workspace.workspaceFolders@endcode
 *  - property type: @code WorkspaceFoldersServerCapabilities@endcode
 */
using WorkspaceFoldersRequest = glz::rpc::method<
        "workspace/workspaceFolders",
        _Void,
        std::variant<null, std::vector<WorkspaceFolder>>
>;

/** DidChangeWorkspaceFolders Notification
 *
 * @version 3.6.0
 *
 * The @code workspace/didChangeWorkspaceFolders@endcode notification is sent
 * from the client to the server to inform the server about workspace folder
 * configuration changes. A server can register for this notification by using
 * either the server capability @code workspace.workspaceFolders.changeNotifications@endcode
 * or by using the dynamic capability registration mechanism. To dynamically
 * register for the @code workspace/didChangeWorkspaceFolders@endcode send a
 * @code client/registerCapability@endcode request from the server to the client.
 * The registration parameter must have a @code registrations@endcode item of the
 * following form, where @code id@endcode is a unique id used to unregister the
 * capability (the example uses a UUID):
 * @code{.json}
 * {
 *     id: "28c6150c-bd7b-11e7-abc4-cec278b6b50a",
 *     method: "workspace/didChangeWorkspaceFolders"
 * }
 * @endcode
 */
using DidChangeWorkspaceFoldersNotification = glz::rpc::method<
        "workspace/didChangeWorkspaceFolders",
        DidChangeWorkspaceFoldersParams,
        _Void
>;

/** WillCreateFiles Request
 *
 * The will create files request is sent from the client to the server before
 * files are actually created as long as the creation is triggered from within the
 * client either by a user action or by applying a workspace edit. The request can
 * return a @code WorkspaceEdit@endcode which will be applied to workspace before
 * the files are created. Hence the @code WorkspaceEdit@endcode can not manipulate
 * the content of the files to be created. Please note that clients might drop
 * results if computing the edit took too long or if a server constantly fails
 * on this request. This is done to keep creates fast and reliable.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.fileOperations.willCreate@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports sending
 * @code workspace/willCreateFiles@endcode notifications.
 *
 * Server Capability:
 *  - property name (optional): @code workspace.fileOperations.willCreate@endcode
 *  - property type: @code FileOperationRegistrationOptions@endcode
 *
 * Registration Options: none
 */
using WillCreateFilesRequest = glz::rpc::method<
        "workspace/willCreateFiles",
        CreateFilesParams,
        std::variant<null, WorkspaceEdit>
>;

/** DidCreateFiles Notification
 *
 * The did create files notification is sent from the client to the server when
 * files were created from within the client.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.fileOperations.didCreate@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports sending
 * @code workspace/didCreateFiles@endcode notifications.
 *
 * Server Capability:
 *  - property name (optional): @code workspace.fileOperations.didCreate@endcode
 *  - property type: @code FileOperationRegistrationOptions@endcode
 *
 * The capability indicates that the server is interested in receiving
 * @code workspace/didCreateFiles@endcode notifications.
 */
using DidCreateFilesNotification = glz::rpc::method<
        "workspace/didCreateFiles",
        CreateFilesParams,
        _Void
>;

/** WillRenameFiles Request
 *
 * The will rename files request is sent from the client to the server before
 * files are actually renamed as long as the rename is triggered from within the
 * client either by a user action or by applying a workspace edit. The request
 * can return a WorkspaceEdit which will be applied to workspace before the files
 * are renamed. Please note that clients might drop results if computing the edit
 * took too long or if a server constantly fails on this request. This is done to
 * keep renames fast and reliable.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.fileOperations.willRename@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports sending
 * @code workspace/willRenameFiles@endcode notifications.
 *
 * Server Capability:
 *  - property name (optional): @code workspace.fileOperations.willRename@endcode
 *  - property type: @code FileOperationRegistrationOptions@endcode
 *
 * The capability indicates that the server is interested in receiving
 * @code workspace/willRenameFiles@endcode notifications.
 *
 * Registration Options: none
 */
using WillRenameFilesRequest = glz::rpc::method<
        "workspace/willRenameFiles",
        RenameFilesParams,
        std::variant<null, WorkspaceEdit>
>;

/** DidRenameFiles Notification
 *
 * The did rename files notification is sent from the client to the server when
 * files were renamed from within the client.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.fileOperations.didRename@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports sending
 * @code workspace/didRenameFiles@endcode notifications.
 *
 * Server Capability:
 *  - property name (optional): @code workspace.fileOperations.didRename@endcode
 *  - property type: @code FileOperationRegistrationOptions@endcode
 *
 * The capability indicates that the server is interested in receiving
 * @code workspace/didRenameFiles@endcode notifications.
 */
using DidRenameFilesNotification = glz::rpc::method<
        "workspace/didRenameFiles",
        RenameFilesParams,
        _Void
>;

/** WillDeleteFiles Request
 *
 * The will delete files request is sent from the client to the server before files
 * are actually deleted as long as the deletion is triggered from within the client
 * either by a user action or by applying a workspace edit. The request can return
 * a WorkspaceEdit which will be applied to workspace before the files are deleted.
 * Please note that clients might drop results if computing the edit took too long
 * or if a server constantly fails on this request. This is done to keep deletes
 * fast and reliable.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.fileOperations.willDelete@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports sending
 * @code workspace/willDeleteFiles@endcode requests.
 *
 * Server Capability:
 *  - property name (optional): @code workspace.fileOperations.willDelete@endcode
 *  - property type: @code FileOperationRegistrationOptions@endcode
 *
 * The capability indicates that the server is interested in receiving
 * @code workspace/willDeleteFiles@endcode requests.
 *
 * Registration Options: none
 */
using WillDeleteFilesRequest = glz::rpc::method<
        "workspace/willDeleteFiles",
        DeleteFilesParams,
        std::variant<null, WorkspaceEdit>
>;

/** DidDeleteFiles Notification
 *
 * The did delete files notification is sent from the client to the server when
 * files were deleted from within the client.
 *
 * Client Capability:
 *  - property name (optional): @code workspace.fileOperations.didDelete@endcode
 *  - property type: @code boolean@endcode
 *
 * The capability indicates that the client supports sending
 * @code workspace/didDeleteFiles@endcode notifications.
 *
 * Server Capability:
 *  - property name (optional): @code workspace.fileOperations.didDelete@endcode
 *  - property type: @code FileOperationRegistrationOptions@endcode
 *
 * The capability indicates that the server is interested in receiving
 * @code workspace/didDeleteFiles@endcode notifications.
 */
using DidDeleteFilesNotification = glz::rpc::method<
        "workspace/didDeleteFiles",
        DeleteFilesParams,
        _Void
>;

/** DidChangeWatchedFiles Notification
 *
 * The watched files notification is sent from the client to the server when the
 * client detects changes to files and folders watched by the language client
 * (note although the name suggest that only file events are sent it is about
 * file system events which include folders as well). It is recommended that
 * servers register for these file system events using the registration mechanism.
 * In former implementations clients pushed file events without the server actively
 * asking for it.
 *
 * Servers are allowed to run their own file system watching mechanism and not rely
 * on clients to provide file system events. However this is not recommended due
 * to the following reasons:
 *  - to our experience getting file system watching on disk right is challenging,
 *    especially if it needs to be supported across multiple OSes.
 *  - file system watching is not for free especially if the implementation uses
 *    some sort of polling and keeps a file system tree in memory to compare time
 *    stamps (as for example some node modules do)
 *  - a client usually starts more than one server. If every server runs its own
 *    file system watching it can become a CPU or memory problem.
 *  - in general there are more server than client implementations. So this problem
 *    is better solved on the client side.
 *
 * Client Capability:
 *  - property path (optional): @code workspace.didChangeWatchedFiles@endcode
 *  - property type: @code DidChangeWatchedFilesClientCapabilities@endcode
 *
 * Registration Options: @code DidChangeWatchedFilesRegistrationOptions@endcode
 */
using DidChangeWatchedFilesNotification = glz::rpc::method<
        "workspace/didChangeWatchedFiles",
        DidChangeWatchedFilesParams,
        _Void
>;

/** Execute a command
 *
 * The @code workspace/executeCommand@endcode request is sent from the client to
 * the server to trigger command execution on the server. In most cases the server
 * creates a @code WorkspaceEdit@endcode structure and applies the changes to the
 * workspace using the request @code workspace/applyEdit@endcode which is sent
 * from the server to the client.
 *
 * Client Capability:
 *  - property path (optional): @code workspace.executeCommand@endcode
 *  - property type: @code ExecuteCommandClientCapabilities@endcode
 *
 * Server Capability:
 *  - property path (optional): @code executeCommandProvider@endcode
 *  - property type: @code ExecuteCommandOptions@endcode
 *
 * Registration Options: @code ExecuteCommandRegistrationOptions@endcode
 */
using ExecuteCommand = glz::rpc::method<
        "workspace/executeCommand",
        ExecuteCommandParams,
        LSPAny
>;

/** Applies a WorkspaceEdit
 *
 * The @code workspace/applyEdit@endcode request is sent from the server to the
 * client to modify resource on the client side.
 *
 * Client Capability:
 *  - property path (optional): @code workspace.applyEdit@endcode
 *  - property type: @code boolean@endcode
 */
using ApplyEdit = glz::rpc::method<
        "workspace/applyEdit",
        ApplyWorkspaceEditParams,
        ApplyWorkspaceEditResult
>;

/** Text Document Content Request
 *
 * @version 3.18.0
 *
 * The @code workspace/textDocumentContent@endcode request is sent from the client
 * to the server to dynamically fetch the content of a text document. Clients
 * should treat the content returned from this requests as readonly.
 *
 * Client Capability:
 *  - property path (optional): @code workspace.textDocumentContent@endcode
 *  - property type: @code TextDocumentContentClientCapabilities@endcode
 *
 * Server Capability:
 *  - property path (optional): @code workspace.textDocumentContent@endcode
 *  - property type: @code TextDocumentContentOptions@endcode
 *
 * Registration Options: @code TextDocumentContentRegistrationOptions@endcode
 */
using TextDocumentContentRequest = glz::rpc::method<
        "workspace/textDocumentContent",
        TextDocumentContentParams,
        TextDocumentContentResult
>;

/** Text Document Content Refresh Request
 *
 * @version 3.18.0
 *
 * The @code workspace/textDocumentContent/refreshrequest@endcode is sent from the
 * server to the client to refresh the content of a specific text document.
 */
using TextDocumentContentRefreshRequest = glz::rpc::method<
        "workspace/textDocumentContent/refresh",
        TextDocumentContentRefreshParams,
        _Void
>;

/** ShowMessage Notification
 *
 * The show message notification is sent from a server to a client to ask the client
 * to display a particular message in the user interface.
 */
using ShowMessageNotification = glz::rpc::method<
        "window/showMessage",
        ShowMessageParams,
        _Void
>;

/** ShowMessage Request
 *
 * The show message request is sent from a server to a client to ask the client
 * to display a particular message in the user interface. In addition to the show
 * message notification the request allows to pass actions and to wait for an answer
 * from the client.
 *
 * Client Capability:
 *  - property path (optional): @code window.showMessage@endcode
 *  - property type: @code ShowMessageRequestClientCapabilities@endcode
 */
using ShowMessageRequest = glz::rpc::method<
        "window/showMessageRequest",
        ShowMessageRequestParams,
        std::variant<null, MessageActionItem>
>;

/** Show Document Request
 *
 * @version 3.16.0
 *
 * The show document request is sent from a server to a client to ask the client
 * to display a particular resource referenced by a URI in the user interface.
 *
 * Client Capability:
 *  - property path (optional): @code window.showDocument@endcode
 *  - property type: @code ShowDocumentClientCapabilities@endcode
 */
using ShowDocumentRequest = glz::rpc::method<
        "window/showDocument",
        ShowDocumentParams,
        ShowDocumentResult
>;

/** LogMessage Notification
 *
 * The log message notification is sent from the server to the client to ask the
 * client to log a particular message.
 */
using LogMessageNotification = glz::rpc::method<
        "window/logMessage",
        LogMessageParams,
        _Void
>;

/** Create Work Done Progress
 *
 * The @code window/workDoneProgress/create@endcode request is sent from the server
 * to the client to ask the client to create a work done progress.
 *
 * Client Capability:
 *  - property name (optional): @code window.workDoneProgress@endcode
 *  - property type: @code boolean@endcode
 */
using CreateWorkDoneProgress = glz::rpc::method<
        "window/workDoneProgress/create",
        WorkDoneProgressCreateParams,
        _Void
>;

/** Cancel a Work Done Progress
 *
 * The @code window/workDoneProgress/cancel@endcode notification is sent from the
 * client to the server to cancel a progress initiated on the server side using the
 * @code window/workDoneProgress/create@endcode. The progress need not be marked
 * as @code cancellable@endcode to be cancelled and a client may cancel a progress
 * for any number of reasons: in case of error, reloading a workspace etc.
 */
using CancelWorkDoneProgress = glz::rpc::method<
        "window/workDoneProgress/cancel",
        WorkDoneProgressCancelParams,
        _Void
>;

/** Telemetry Notification
 *
 * The telemetry notification is sent from the server to the client to ask the
 * client to log a telemetry event. The protocol doesn't specify the payload since
 * no interpretation of the data happens in the protocol. Most clients even don't
 * handle the event directly but forward them to the extensions owing the corresponding
 * server issuing the event.
 */
using TelemetryNotification = glz::rpc::method<
        "telemetry/event",
        glz::generic,
        _Void
>;
