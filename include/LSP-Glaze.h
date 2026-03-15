//
// Created by stort on 08/08/2025.
//

#pragma once

#include <glaze/glaze.hpp>

#include <LSP-Protocol.h>

#define GLAZE_META(__type__, ...)                               \
template<>                                                      \
struct glz::meta<__type__> {                                    \
        static constexpr auto value = glz::object(__VA_ARGS__); \
}

#define MAKE_TYPE_PAIR(__type__, ...)                           \
__VA_OPT__(EXPAND(__MAKE_TYPE_PAIR(__type__, __VA_ARGS__)))

#define __MAKE_TYPE_PAIR(__type__, __name__, ...)                                               \
#__name__, &__type__::__name__ __VA_OPT__(, __MAKE_TYPE_PAIR2 PARENS (__type__, __VA_ARGS__))

#define __MAKE_TYPE_PAIR2() __MAKE_TYPE_PAIR

#define GLAZE_META_NORMAL(__type__, ...) GLAZE_META(__type__, MAKE_TYPE_PAIR(__type__, __VA_ARGS__))

#define STATIC_FIELD(__type__, __name__) #__name__, [](auto &&) -> std::add_lvalue_reference_t<std::add_const_t<decltype(__type__::__name__)>> { return __type__::__name__; }

#define GLAZE_STRING_ENUM_META(__type__)                \
template<>                                              \
struct glz::meta<__type__> {                            \
        static constexpr auto value = &__type__::value; \
}

template<>
struct glz::meta<_Void> {
        static constexpr auto value = glz::object("______", [](auto &&) { return "_____"; } );
};

GLAZE_STRING_ENUM_META(ResourceOperationKind);
GLAZE_STRING_ENUM_META(FailureHandlingKind);

GLAZE_META_NORMAL(WorkspaceEditClientCapabilities::_ChangeAnnotationSupport,
        groupsOnLabel
);

GLAZE_META_NORMAL(WorkspaceEditClientCapabilities,
        documentChanges,
        resourceOperations,
        failureHandling,
        normalizesLineEndings,
        changeAnnotationSupport,
        metadataSupport,
        snippetEditSupport
);

GLAZE_META_NORMAL(DidChangeConfigurationClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(DidChangeWatchedFilesClientCapabilities,
        dynamicRegistration,
        relativePatternSupport
);

GLAZE_META_NORMAL(WorkspaceSymbolClientCapabilities::_SymbolKind,
        valueSet
);

GLAZE_META_NORMAL(WorkspaceSymbolClientCapabilities::_TagSupport,
        valueSet
);

GLAZE_META_NORMAL(WorkspaceSymbolClientCapabilities::_ResolveSupport,
        properties
);

GLAZE_META_NORMAL(WorkspaceSymbolClientCapabilities,
        dynamicRegistration,
        symbolKind,
        tagSupport,
        resolveSupport
);

GLAZE_META_NORMAL(ExecuteCommandClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(SemanticTokensWorkspaceClientCapabilities,
        refreshSupport
);

GLAZE_META_NORMAL(CodeLensWorkspaceClientCapabilities,
        refreshSupport
);

GLAZE_META_NORMAL(InlineValueWorkspaceClientCapabilities,
        refreshSupport
);

GLAZE_META_NORMAL(InlayHintWorkspaceClientCapabilities,
        refreshSupport
);

GLAZE_META_NORMAL(DiagnosticWorkspaceClientCapabilities,
        refreshSupport
);

GLAZE_META_NORMAL(TextDocumentContentClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(FoldingRangeWorkspaceClientCapabilities,
        refreshSupport
);

GLAZE_META_NORMAL(TextDocumentSyncClientCapabilities,
        dynamicRegistration,
        willSave,
        willSaveWaitUntil,
        didSave
);

GLAZE_STRING_ENUM_META(MarkupKind);

GLAZE_META_NORMAL(CompletionClientCapabilities::_CompletionItem::_CompletionTagSupport,
        valueSet
);

GLAZE_META_NORMAL(CompletionClientCapabilities::_CompletionItem::_ResolveSupport,
        properties
);

GLAZE_META_NORMAL(CompletionClientCapabilities::_CompletionItem::_InsertTextModeSupport,
        valueSet
);

GLAZE_META_NORMAL(CompletionClientCapabilities::_CompletionItem,
        snippetSupport,
        commitCharactersSupport,
        documentationFormat,
        deprecatedSupport,
        preselectSupport,
        tagSupport,
        insertReplaceSupport,
        resolveSupport,
        insertTextModeSupport,
        labelDetailsSupport
);

GLAZE_META_NORMAL(CompletionClientCapabilities::_CompletionItemKind,
        valueSet
);

GLAZE_META_NORMAL(CompletionClientCapabilities::_CompletionList,
        itemDefaults,
        applyKindSupport
);

GLAZE_META_NORMAL(CompletionClientCapabilities,
        dynamicRegistration,
        completionItem,
        completionItemKind,
        contextSupport,
        insertTextMode,
        completionList
);

GLAZE_META_NORMAL(HoverClientCapabilities,
        dynamicRegistration,
        contentFormat
);

GLAZE_META_NORMAL(SignatureHelpClientCapabilities::_SignatureInformation::_ParameterInformation,
        labelOffsetSupport
);

GLAZE_META_NORMAL(SignatureHelpClientCapabilities::_SignatureInformation,
        documentationFormat,
        parameterInformation,
        activeParameterSupport,
        noActiveParameterSupport,
);

GLAZE_META_NORMAL(SignatureHelpClientCapabilities,
        dynamicRegistration,
        signatureInformation,
        contextSupport
);

GLAZE_META_NORMAL(DeclarationClientCapabilities,
        dynamicRegistration,
        linkSupport
);

GLAZE_META_NORMAL(DefinitionClientCapabilities,
        dynamicRegistration,
        linkSupport
);

GLAZE_META_NORMAL(TypeDefinitionClientCapabilities,
        dynamicRegistration,
        linkSupport
);

GLAZE_META_NORMAL(ImplementationClientCapabilities,
        dynamicRegistration,
        linkSupport
);

GLAZE_META_NORMAL(ReferenceClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(DocumentHighlightClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(DocumentSymbolClientCapabilities::_SymbolKind,
        valueSet
);

GLAZE_META_NORMAL(DocumentSymbolClientCapabilities::_TagSupport,
        valueSet
);

GLAZE_META_NORMAL(DocumentSymbolClientCapabilities,
        dynamicRegistration,
        symbolKind,
        hierarchicalDocumentSymbolSupport,
        tagSupport,
        labelSupport
);

GLAZE_STRING_ENUM_META(CodeActionKind);

GLAZE_META_NORMAL(decltype(CodeActionClientCapabilities::_CodeActionLiteralSupport::codeActionKind),
        valueSet
);

GLAZE_META_NORMAL(CodeActionClientCapabilities::_CodeActionLiteralSupport,
        codeActionKind
);

GLAZE_META_NORMAL(CodeActionClientCapabilities::_TagSupport,
        valueSet
);

GLAZE_META_NORMAL(CodeActionClientCapabilities,
        dynamicRegistration,
        codeActionLiteralSupport,
        isPreferredSupport,
        disabledSupport,
        dataSupport,
        resolveSupport,
        honorsChangeAnnotations,
        documentationSupport,
        tagSupport
);

GLAZE_META_NORMAL(ClientCodeLensResolveOptions,
        properties
);

GLAZE_META_NORMAL(CodeLensClientCapabilities,
        dynamicRegistration,
        resolveSupport
);

GLAZE_META_NORMAL(DocumentLinkClientCapabilities,
        dynamicRegistration,
        tooltipSupport
);

GLAZE_META_NORMAL(DocumentColorClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(DocumentFormattingClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(DocumentRangeFormattingClientCapabilities,
        dynamicRegistration,
        rangesSupport
);

GLAZE_META_NORMAL(DocumentOnTypeFormattingClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(RenameClientCapabilities,
        dynamicRegistration,
        prepareSupport,
        prepareSupportDefaultBehavior,
        honorsChangeAnnotations
);

GLAZE_META_NORMAL(PublishDiagnosticsClientCapabilities::_TagSupport,
        valueSet
);

GLAZE_META_NORMAL(PublishDiagnosticsClientCapabilities,
        relatedInformation,
        tagSupport,
        versionSupport,
        codeDescriptionSupport,
        dataSupport
);

GLAZE_STRING_ENUM_META(FoldingRangeKind);

GLAZE_META_NORMAL(FoldingRangeClientCapabilities::_FoldingRangeKind,
        valueSet
);

GLAZE_META_NORMAL(FoldingRangeClientCapabilities::_FoldingRange,
        collapsedText
);

GLAZE_META_NORMAL(FoldingRangeClientCapabilities,
        dynamicRegistration,
        rangeLimit,
        lineFoldingOnly,
        foldingRangeKind,
        foldingRange
);

GLAZE_META_NORMAL(SelectionRangeClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(LinkedEditingRangeClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(CallHierarchyClientCapabilities,
        dynamicRegistration
);

GLAZE_STRING_ENUM_META(TokenFormat);

GLAZE_META_NORMAL(decltype(SemanticTokensClientCapabilities::requests)::_Range
);

GLAZE_META_NORMAL(decltype(SemanticTokensClientCapabilities::requests)::_Full,
        delta
);

GLAZE_META_NORMAL(decltype(SemanticTokensClientCapabilities::requests),
        range,
        full
);

GLAZE_META_NORMAL(SemanticTokensClientCapabilities,
        dynamicRegistration,
        requests,
        tokenTypes,
        tokenModifiers,
        formats,
        overlappingTokenSupport,
        multilineTokenSupport,
        serverCancelSupport,
        augmentsSyntaxTokens
);

GLAZE_META_NORMAL(MonikerClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(TypeHierarchyClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(InlineValueClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(InlayHintClientCapabilities::_ResolveSupport,
        properties
);

GLAZE_META_NORMAL(InlayHintClientCapabilities,
        dynamicRegistration,
        resolveSupport
);

GLAZE_META_NORMAL(DiagnosticClientCapabilities,
        dynamicRegistration,
        relatedDocumentSupport,
        markupMessageSupport
);

GLAZE_META_NORMAL(TextDocumentFilterClientCapabilities,
        relativePatternSupport
);

GLAZE_META_NORMAL(InlineCompletionClientCapabilities,
        dynamicRegistration
);

GLAZE_META_NORMAL(TextDocumentClientCapabilities,
        synchronization,
        filters,
        completion,
        hover,
        signatureHelp,
        declaration,
        definition,
        typeDefinition,
        implementation,
        references,
        documentHighlight,
        documentSymbol,
        codeAction,
        codeLens,
        documentLink,
        colorProvider,
        formatting,
        rangeFormatting,
        onTypeFormatting,
        rename,
        publishDiagnostics,
        foldingRange,
        selectionRange,
        linkedEditingRange,
        callHierarchy,
        semanticTokens,
        moniker,
        typeHierarchy,
        inlineValue,
        inlayHint,
        diagnostic,
        inlineCompletion
);

GLAZE_META_NORMAL(NotebookDocumentSyncClientCapabilities,
        dynamicRegistration,
        executionSummarySupport
);

GLAZE_META_NORMAL(NotebookDocumentClientCapabilities,
        synchronization
);

GLAZE_META_NORMAL(ShowMessageRequestClientCapabilities::_MessageActionItem,
        additionalPropertiesSupport
);

GLAZE_META_NORMAL(ShowMessageRequestClientCapabilities,
        messageActionItem
);

GLAZE_META_NORMAL(ShowDocumentClientCapabilities,
        support
);

GLAZE_META_NORMAL(RegularExpressionsClientCapabilities,
        engine,
        version
);

GLAZE_META_NORMAL(MarkdownClientCapabilities,
        parser,
        version,
        allowedTags
);

GLAZE_STRING_ENUM_META(PositionEncodingKind);

GLAZE_META_NORMAL(ClientCapabilities::_Workspace::_FileOperations,
        dynamicRegistration,
        didCreate,
        willCreate,
        didRename,
        willRename,
        didDelete,
        willDelete
);

GLAZE_META_NORMAL(ClientCapabilities::_Workspace,
        applyEdit,
        workspaceEdit,
        didChangeConfiguration,
        didChangeWatchedFiles,
        symbol,
        executeCommand,
        workspaceFolders,
        configuration,
        semanticTokens,
        codeLens,
        fileOperations,
        inlineValue,
        inlayHint,
        diagnostics,
        textDocumentContent,
        foldingRange
);

GLAZE_META_NORMAL(ClientCapabilities::_Window,
        workDoneProgress,
        showMessage,
        showDocument
);

GLAZE_META_NORMAL(ClientCapabilities::_General::_StaleRequestSupport,
        cancel,
        retryOnContentModified
);

GLAZE_META_NORMAL(ClientCapabilities::_General,
        staleRequestSupport,
        regularExpressions,
        markdown,
        positionEncodings
);

GLAZE_META_NORMAL(ClientCapabilities,
        workspace,
        textDocument,
        notebookDocument,
        window,
        general,
        experimental
);

GLAZE_STRING_ENUM_META(TraceValue);

GLAZE_META_NORMAL(WorkspaceFolder,
        uri,
        name
);

GLAZE_META_NORMAL(WorkDoneProgressParams,
        workDoneToken
);

GLAZE_META_NORMAL(InitializeParams::_ClientInfo,
        name,
        version
);

GLAZE_META_NORMAL(InitializeParams,
        workDoneToken,  // Inherited from WorkDoneProgressParams
        processId,
        clientInfo,
        locale,
        rootPath,
        rootUri,
        initializationOptions,
        capabilities,
        trace,
        workspaceFolders
);

GLAZE_META_NORMAL(TextDocumentSyncOptions,
        openClose,
        change
);

GLAZE_META_NORMAL(_NotebookDocumentFilter1,
        notebookType,
        scheme,
        pattern
);

GLAZE_META_NORMAL(_NotebookDocumentFilter2,
        notebookType,
        scheme,
        pattern
);

GLAZE_META_NORMAL(_NotebookDocumentFilter3,
        notebookType,
        scheme,
        pattern
);

GLAZE_META_NORMAL(NotebookDocumentSyncOptions::_Cell,
        language
);

GLAZE_META_NORMAL(NotebookDocumentSyncOptions::_NotebookSelector1,
        notebook,
        cells
);

GLAZE_META_NORMAL(NotebookDocumentSyncOptions::_NotebookSelector2,
        notebook,
        cells
);

GLAZE_META_NORMAL(NotebookDocumentSyncOptions,
        notebookSelector,
        save
);

GLAZE_META_NORMAL(StaticRegistrationOptions,
        id
);

GLAZE_META_NORMAL(NotebookDocumentSyncRegistrationOptions,
        notebookSelector,  // Inherited from NotebookDocumentSyncOptions
        save,              // Inherited from NotebookDocumentSyncOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(WorkDoneProgressOptions,
        workDoneProgress
);

GLAZE_META_NORMAL(CompletionOptions::_CompletionItem,
        labelDetailsSupport
);

GLAZE_META_NORMAL(CompletionOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        triggerCharacters,
        allCommitCharacters,
        resolveProvider,
        completionItem
);

GLAZE_META_NORMAL(HoverOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(SignatureHelpOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        triggerCharacters,
        retriggerCharacters
);

GLAZE_META_NORMAL(DeclarationOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(DocumentFilter,
        language,
        scheme,
        pattern
);

GLAZE_META_NORMAL(TextDocumentRegistrationOptions,
        documentSelector
);

GLAZE_META_NORMAL(DeclarationRegistrationOptions,
        workDoneProgress,  // Inherited from DeclarationOptions
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(DefinitionOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(TypeDefinitionOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(TypeDefinitionRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        workDoneProgress,  // Inherited from TypeDefinitionOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(ImplementationOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(ImplementationRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        workDoneProgress,  // Inherited from ImplementationOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(ReferenceOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(DocumentHighlightOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(DocumentSymbolOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        label
);

GLAZE_META_NORMAL(CodeActionKindDocumentation,
        kind,
        command
);

GLAZE_META_NORMAL(CodeActionOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        codeActionKinds,
        documentation,
        resolveProvider
);

GLAZE_META_NORMAL(CodeLensOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        resolveProvider
);

GLAZE_META_NORMAL(DocumentLinkOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        resolveProvider
);

GLAZE_META_NORMAL(DocumentColorOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(DocumentColorRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        id,                // Inherited from StaticRegistrationOptions
        workDoneProgress   // Inherited from DocumentColorOptions
);

GLAZE_META_NORMAL(DocumentFormattingOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(DocumentRangeFormattingOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        rangesSupport
);

GLAZE_META_NORMAL(DocumentOnTypeFormattingOptions,
        firstTriggerCharacter,
        moreTriggerCharacters
);

GLAZE_META_NORMAL(RenameOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        prepareProvider
);

GLAZE_META_NORMAL(FoldingRangeOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(FoldingRangeRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        workDoneProgress,  // Inherited from FoldingRangeOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(ExecuteCommandOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        commands
);

GLAZE_META_NORMAL(SelectionRangeOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(SelectionRangeRegistrationOptions,
        workDoneProgress,  // Inherited from SelectionRangeOptions
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(LinkedEditingRangeOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(LinkedEditingRangeRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        workDoneProgress,  // Inherited from LinkedEditingRangeOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(CallHierarchyOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(CallHierarchyRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        workDoneProgress,  // Inherited from CallHierarchyOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(SemanticTokensLegend,
        tokenTypes,
        tokenModifiers
);

GLAZE_META_NORMAL(SemanticTokensOptions::_Empty,
);

GLAZE_META_NORMAL(SemanticTokensOptions::_Full,
        delta
);

GLAZE_META_NORMAL(SemanticTokensOptions,
        workDoneProgress,  // Inherited from CallHierarchyOptions
        legend,
        range,
        full
);

GLAZE_META_NORMAL(SemanticTokensRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        workDoneProgress,  // Inherited from SemanticTokensOptions
        legend,            // Inherited from SemanticTokensOptions
        range,             // Inherited from SemanticTokensOptions
        full,              // Inherited from SemanticTokensOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(MonikerOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(MonikerRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        workDoneProgress   // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(TypeHierarchyOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(TypeHierarchyRegistrationOptions,
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        workDoneProgress,  // Inherited from TypeHierarchyOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(InlineValueOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(InlineValueRegistrationOptions,
        workDoneProgress,  // Inherited from InlineValueOptions
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(InlayHintOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        resolveProvider
);

GLAZE_META_NORMAL(InlayHintRegistrationOptions,
        workDoneProgress,  // Inherited from InlayHintOptions
        resolveProvider,   // Inherited from InlayHintOptions
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(DiagnosticOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        identifier,
        internalDependencies,
        workspaceDiagnostics
);

GLAZE_META_NORMAL(DiagnosticRegistrationOptions,
        documentSelector,      // Inherited from TextDocumentRegistrationOptions
        workDoneProgress,      // Inherited from DiagnosticOptions
        identifier,            // Inherited from DiagnosticOptions
        internalDependencies,  // Inherited from DiagnosticOptions
        workspaceDiagnostics,  // Inherited from DiagnosticOptions
        id                     // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(WorkspaceSymbolOptions,
        workDoneProgress,  // Inherited from WorkDoneProgressOptions
        resolveProvider
);

GLAZE_META_NORMAL(WorkspaceFoldersServerCapabilities,
        supported,
        changeNotifications
);

GLAZE_META_NORMAL(TextDocumentContentOptions,
        schemes
);

GLAZE_META_NORMAL(TextDocumentContentRegistrationOptions,
        schemes,  // Inherited from TextDocumentContentOptions
        id        // Inherited from StaticRegistrationOptions
);

GLAZE_STRING_ENUM_META(FileOperationPatternKind);

GLAZE_META_NORMAL(FileOperationPatternOptions,
        ignoreCase
);

GLAZE_META_NORMAL(FileOperationPattern,
        glob,
        matches,
        options
);

GLAZE_META_NORMAL(FileOperationFilter,
        scheme,
        pattern
);

GLAZE_META_NORMAL(FileOperationRegistrationOptions,
        filters
);

GLAZE_META_NORMAL(InlineCompletionOptions,
        workDoneProgress  // Inherited from WorkDoneProgressOptions
);

GLAZE_META_NORMAL(InlineCompletionRegistrationOptions,
        workDoneProgress,  // Inherited from InlineCompletionOptions
        documentSelector,  // Inherited from TextDocumentRegistrationOptions
        id                 // Inherited from StaticRegistrationOptions
);

GLAZE_META_NORMAL(ServerCapabilities::_Workspace::_FileOperation,
        didCreate,
        willCreate,
        didRename,
        willRename,
        didDelete,
        willDelete
);

GLAZE_META_NORMAL(ServerCapabilities::_Workspace,
        workspaceFolders,
        fileOperations,
        textDocumentContent
);

GLAZE_META_NORMAL(ServerCapabilities::_TextDocument::_Diagnostic,
        markupMessageSupport
);

GLAZE_META_NORMAL(ServerCapabilities::_TextDocument,
        diagnostic
);

GLAZE_META_NORMAL(ServerCapabilities,
        positionEncoding,
        textDocumentSync,
        notebookDocumentSync,
        completionProvider,
        hoverProvider,
        signatureHelpProvider,
        declarationProvider,
        definitionProvider,
        typeDefinitionProvider,
        implementationProvider,
        referencesProvider,
        documentHighlightProvider,
        documentSymbolProvider,
        codeActionProvider,
        codeLensProvider,
        documentLinkProvider,
        colorProvider,
        documentFormattingProvider,
        documentRangeFormattingProvider,
        documentOnTypeFormattingProvider,
        renameProvider,
        foldingRangeProvider,
        executeCommandProvider,
        selectionRangeProvider,
        linkedEditingRangeProvider,
        callHierarchyProvider,
        semanticTokensProvider,
        monikerProvider,
        typeHierarchyProvider,
        inlineValueProvider,
        inlayHintProvider,
        diagnosticProvider,
        workspaceSymbolProvider,
        inlineCompletionProvider,
        textDocument,
        workspace,
        experimental
);

GLAZE_META_NORMAL(ServerInfo,
        name,
        version
);

GLAZE_META_NORMAL(InitializeResult,
        capabilities,
        serverInfo
);

GLAZE_META_NORMAL(InitializedParams
);

GLAZE_META_NORMAL(Registration,
        id,
        method,
        registerOptions
);

GLAZE_META_NORMAL(RegistrationParams,
        registrations
);

GLAZE_META_NORMAL(Unregistration,
        id,
        method
);

GLAZE_META_NORMAL(UnregistrationParams,
        unregisterations
);

GLAZE_META_NORMAL(SetTraceParams,
        value
);

GLAZE_META_NORMAL(LogTraceParams,
        message,
        verbose
);

GLAZE_META_NORMAL(TextDocumentItem,
        uri,
        languageId,
        version,
        text
);

GLAZE_META_NORMAL(DidOpenTextDocumentParams,
        textDocument
);

GLAZE_META_NORMAL(TextDocumentIdentifier,
        uri
);

GLAZE_META_NORMAL(VersionedTextDocumentIdentifier,
        uri,  // Inherited from TextDocumentIdentifier
        version
);

GLAZE_META_NORMAL(Position,
        line,
        character
);

GLAZE_META_NORMAL(Range,
        start,
        end
);

GLAZE_META_NORMAL(TextDocumentContentChangeEvent::_Text,
        text
);

GLAZE_META_NORMAL(TextDocumentContentChangeEvent,
        range,
        rangeLength,
        text
);

GLAZE_META_NORMAL(DidChangeTextDocumentParams,
        textDocument,
        contentChanges
);

GLAZE_META_NORMAL(WillSaveTextDocumentParams,
        textDocument,
        reason
);

GLAZE_META_NORMAL(TextEdit,
        range,
        newText
);

GLAZE_META_NORMAL(DidSaveTextDocumentParams,
        textDocument,
        text
);

GLAZE_META_NORMAL(DidCloseTextDocumentParams,
        textDocument
);

GLAZE_META_NORMAL(ExecutionSummary,
        executionOrder,
        success
);

GLAZE_META_NORMAL(NotebookCell,
        kind,
        document,
        metadata,
        executionSummary
);

GLAZE_META_NORMAL(NotebookDocument,
        uri,
        notebookType,
        version,
        metadata,
        cells
);

GLAZE_META_NORMAL(DidOpenNotebookDocumentParams,
        notebookDocument,
        cellTextDocuments
);

GLAZE_META_NORMAL(VersionedNotebookDocumentIdentifier,
        version,
        uri
);

GLAZE_META_NORMAL(NotebookCellArrayChange,
        start,
        deleteCount,
        cells
);

GLAZE_META_NORMAL(NotebookDocumentChangeEvent::_Cells::_Structure,
        array,
        didOpen,
        didClose
);

GLAZE_META_NORMAL(NotebookDocumentChangeEvent::_Cells::_TextContent,
        document,
        changes
);

GLAZE_META_NORMAL(NotebookDocumentChangeEvent::_Cells,
        structure,
        data,
        textContent
);

GLAZE_META_NORMAL(NotebookDocumentChangeEvent,
        metadata,
        cells
);

GLAZE_META_NORMAL(DidChangeNotebookDocumentParams,
        notebookDocument,
        change
);

GLAZE_META_NORMAL(NotebookDocumentIdentifier,
        uri
);

GLAZE_META_NORMAL(DidSaveNotebookDocumentParams,
        notebookDocument
);

GLAZE_META_NORMAL(DidCloseNotebookDocumentParams,
        notebookDocument,
        cellTextDocuments
);

GLAZE_META_NORMAL(TextDocumentPositionParams,
        textDocument,
        position
);

GLAZE_META_NORMAL(PartialResultParams,
        partialResultToken
);

GLAZE_META_NORMAL(DeclarationParams,
        textDocument,       // Inherited from TextDocumentPositionParams
        position,           // Inherited from TextDocumentPositionParams
        workDoneToken,      // Inherited from WorkDoneProgressParams
        partialResultToken  // Inherited from PartialResultParams
);

GLAZE_META_NORMAL(Location,
        uri,
        range
);

GLAZE_META_NORMAL(LocationLink,
        originSelectionRange,
        targetUri,
        targetRange,
        targetSelectionRange
);

GLAZE_META_NORMAL(DefinitionParams,
        textDocument,       // Inherited from TextDocumentPositionParams
        position,           // Inherited from TextDocumentPositionParams
        workDoneToken,      // Inherited from WorkDoneProgressParams
        partialResultToken  // Inherited from PartialResultParams
);

GLAZE_META_NORMAL(TypeDefinitionParams,
        textDocument,       // Inherited from TextDocumentPositionParams
        position,           // Inherited from TextDocumentPositionParams
        workDoneToken,      // Inherited from WorkDoneProgressParams
        partialResultToken  // Inherited from PartialResultParams
);

GLAZE_META_NORMAL(ImplementationParams,
        textDocument,       // Inherited from TextDocumentPositionParams
        position,           // Inherited from TextDocumentPositionParams
        workDoneToken,      // Inherited from WorkDoneProgressParams
        partialResultToken  // Inherited from PartialResultParams
);

GLAZE_META_NORMAL(ReferenceContext,
        includeDeclaration
);

GLAZE_META_NORMAL(ReferenceParams,
        textDocument,        // Inherited from TextDocumentPositionParams
        position,            // Inherited from TextDocumentPositionParams
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        context
);

GLAZE_META_NORMAL(CallHierarchyPrepareParams,
        textDocument,  // Inherited from TextDocumentPositionParams
        position,      // Inherited from TextDocumentPositionParams
        workDoneToken  // Inherited from WorkDoneProgressParams
);

GLAZE_META_NORMAL(CallHierarchyItem,
        name,
        kind,
        tags,
        detail,
        uri,
        range,
        selectionRange,
        data
);

GLAZE_META_NORMAL(CallHierarchyIncomingCallsParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        item
);

GLAZE_META_NORMAL(CallHierarchyIncomingCall,
        from,
        fromRanges
);

GLAZE_META_NORMAL(CallHierarchyOutgoingCallsParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        item
);

GLAZE_META_NORMAL(CallHierarchyOutgoingCall,
        to,
        fromRanges
);

GLAZE_META_NORMAL(TypeHierarchyPrepareParams,
        textDocument,  // Inherited from TextDocumentPositionParams
        position,      // Inherited from TextDocumentPositionParams
        workDoneToken  // Inherited from WorkDoneProgressParams
);

GLAZE_META_NORMAL(TypeHierarchyItem,
        name,
        kind,
        tags,
        detail,
        uri,
        range,
        selectionRange,
        data
);

GLAZE_META_NORMAL(TypeHierarchySupertypesParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        item
);

GLAZE_META_NORMAL(TypeHierarchySubtypesParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        item
);

GLAZE_META_NORMAL(DocumentHighlightParams,
        textDocument,       // Inherited from TextDocumentPositionParams
        position,           // Inherited from TextDocumentPositionParams
        workDoneToken,      // Inherited from WorkDoneProgressParams
        partialResultToken  // Inherited from PartialResultParams
);

GLAZE_META_NORMAL(DocumentHighlight,
        range,
        kind
);

GLAZE_META_NORMAL(DocumentLinkParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument
);

GLAZE_META_NORMAL(DocumentLink,
        range,
        target,
        tooltip,
        data
);

GLAZE_META_NORMAL(HoverParams,
        textDocument,  // Inherited from TextDocumentPositionParams
        position,      // Inherited from TextDocumentPositionParams
        workDoneToken  // Inherited from WorkDoneProgressParams
);

GLAZE_META_NORMAL(MarkupContent,
        kind,
        value
);

GLAZE_META_NORMAL(_MarkedString,
        language,
        value
);

GLAZE_META_NORMAL(Hover,
        contents,
        range
);

GLAZE_META_NORMAL(CodeLensParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument
);

GLAZE_META_NORMAL(Command,
        title,
        command,
        arguments
);

GLAZE_META_NORMAL(CodeLens,
        range,
        command,
        data
);

GLAZE_META_NORMAL(FoldingRangeParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument
);

GLAZE_META_NORMAL(FoldingRange,
        startLine,
        startCharacter,
        endLine,
        endCharacter,
        kind,
        collapsedText
);

GLAZE_META_NORMAL(SelectionRangeParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument,
        positions
);

GLAZE_META_NORMAL(SelectionRange,
        range,
        parent
);

GLAZE_META_NORMAL(DocumentSymbolParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument
);

GLAZE_META_NORMAL(DocumentSymbol,
        name,
        detail,
        kind,
        tags,
        deprecated,
        range,
        selectionRange,
        children
);

GLAZE_META_NORMAL(SymbolInformation,
        name,
        kind,
        tags,
        deprecated,
        location,
        containerName
);

GLAZE_META_NORMAL(SemanticTokensParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument
);

GLAZE_META_NORMAL(SemanticTokens,
        resultId,
        data
);

GLAZE_META_NORMAL(SemanticTokensPartialResult,
        data
);

GLAZE_META_NORMAL(SemanticTokensDeltaParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument,
        previousResultId
);

GLAZE_META_NORMAL(SemanticTokensEdit,
        start,
        deleteCount,
        data
);

GLAZE_META_NORMAL(SemanticTokensDelta,
        resultId,
        edits
);

GLAZE_META_NORMAL(SemanticTokensDeltaPartialResult,
        edits
);

GLAZE_META_NORMAL(SemanticTokensRangeParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument,
        range
);

GLAZE_META_NORMAL(InlayHintParams,
        workDoneToken,  // Inherited from WorkDoneProgressParams
        textDocument,
        range
);

GLAZE_META_NORMAL(InlayHintLabelPart,
        value,
        tooltip,
        location,
        command
);

GLAZE_META_NORMAL(InlayHint,
        position,
        label,
        kind,
        textEdits,
        tooltip,
        paddingLeft,
        paddingRight,
        data
);

GLAZE_META_NORMAL(InlineValueContext,
        frameId,
        stoppedLocation
);

GLAZE_META_NORMAL(InlineValueParams,
        workDoneToken,  // Inherited from WorkDoneProgressParams
        textDocument,
        range,
        context
);

GLAZE_META_NORMAL(InlineValueVariableLookup,
        range,
        variableName,
        caseSensitiveLookup
);

GLAZE_META_NORMAL(InlineValueEvaluatableExpression,
        range,
        expression
);

GLAZE_META_NORMAL(MonikerParams,
        textDocument,       // Inherited from TextDocumentPositionParams
        position,           // Inherited from TextDocumentPositionParams
        workDoneToken,      // Inherited from WorkDoneProgressParams
        partialResultToken  // Inherited from PartialResultParams
);

GLAZE_STRING_ENUM_META(UniquenessLevel);
GLAZE_STRING_ENUM_META(MonikerKind);

GLAZE_META_NORMAL(Moniker,
        scheme,
        identifier,
        unique,
        kind
);

GLAZE_META_NORMAL(CompletionContext,
        triggerKind,
        triggerCharacter
);

GLAZE_META_NORMAL(CompletionParams,
        textDocument,        // Inherited from TextDocumentPositionParams
        position,            // Inherited from TextDocumentPositionParams
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        context
);

GLAZE_META_NORMAL(CompletionItemLabelDetails,
        detail,
        description
);

GLAZE_META_NORMAL(InsertReplaceEdit,
        newText,
        insert,
        replace
);

GLAZE_META_NORMAL(CompletionItem,
        label,
        labelDetails,
        kind,
        tags,
        detail,
        documentation,
        deprecated,
        preselect,
        sortText,
        filterText,
        insertText,
        insertTextFormat,
        insertTextMode,
        textEdit,
        textEditText,
        additionalTextEdits,
        commitCharacters,
        command,
        data
);

GLAZE_META_NORMAL(CompletionList::_ItemDefaults::_EditRange,
        insert,
        replace
);

GLAZE_META_NORMAL(CompletionList::_ItemDefaults,
        commitCharacters,
        editRange,
        insertTextFormat,
        insertTextMode,
        data
);

GLAZE_META_NORMAL(CompletionList::_ApplyKind,
        commitCharacters,
        data
);

GLAZE_META_NORMAL(CompletionList,
        isIncomplete,
        itemDefaults,
        applyKind,
        items
);

GLAZE_META_NORMAL(CodeDescription,
        href
);

GLAZE_META_NORMAL(DiagnosticRelatedInformation,
        location,
        message
);

GLAZE_META_NORMAL(Diagnostic,
        range,
        severity,
        code,
        codeDescription,
        source,
        message,
        tags,
        relatedInformation,
        data
);

GLAZE_META_NORMAL(PublishDiagnosticsParams,
        uri,
        version,
        diagnostics
);

GLAZE_META_NORMAL(DocumentDiagnosticParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument,
        identifier,
        previousResultId
);

GLAZE_STRING_ENUM_META(DocumentDiagnosticReportKind);

GLAZE_META(FullDocumentDiagnosticReport,
        STATIC_FIELD(FullDocumentDiagnosticReport, kind),
        MAKE_TYPE_PAIR(FullDocumentDiagnosticReport,
                resultId,
                items
        )
);

GLAZE_META(UnchangedDocumentDiagnosticReport,
        STATIC_FIELD(UnchangedDocumentDiagnosticReport, kind),
        MAKE_TYPE_PAIR(UnchangedDocumentDiagnosticReport,
                resultId
        )
);

GLAZE_META(RelatedFullDocumentDiagnosticReport,
        STATIC_FIELD(RelatedFullDocumentDiagnosticReport, kind),  // Inherited from FullDocumentDiagnosticReport
        MAKE_TYPE_PAIR(RelatedFullDocumentDiagnosticReport,
                resultId,  // Inherited from FullDocumentDiagnosticReport
                items,     // Inherited from FullDocumentDiagnosticReport
                relatedDocuments
        )
);

GLAZE_META(RelatedUnchangedDocumentDiagnosticReport,
        STATIC_FIELD(RelatedUnchangedDocumentDiagnosticReport, kind),  // Inherited from UnchangedDocumentDiagnosticReport
        MAKE_TYPE_PAIR(RelatedUnchangedDocumentDiagnosticReport,
                resultId,  // Inherited from UnchangedDocumentDiagnosticReport
                relatedDocuments
        )
);

GLAZE_META_NORMAL(DocumentDiagnosticReportPartialResult,
        relatedDocuments
);

GLAZE_META_NORMAL(DiagnosticServerCancellationData,
        retriggerRequest
);

GLAZE_META_NORMAL(PreviousResultId,
        uri,
        value
);

GLAZE_META_NORMAL(WorkspaceDiagnosticParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        identifier,
        previousResultIds
);

GLAZE_META(WorkspaceFullDocumentDiagnosticReport,
        STATIC_FIELD(WorkspaceFullDocumentDiagnosticReport, kind),  // Inherited from FullDocumentDiagnosticReport
        MAKE_TYPE_PAIR(WorkspaceFullDocumentDiagnosticReport,
                resultId,  // Inherited from FullDocumentDiagnosticReport
                items,     // Inherited from FullDocumentDiagnosticReport
                uri,
                version
        )
);

GLAZE_META(WorkspaceUnchangedDocumentDiagnosticReport,
        STATIC_FIELD(WorkspaceUnchangedDocumentDiagnosticReport, kind),  // Inherited from UnchangedDocumentDiagnosticReport
        MAKE_TYPE_PAIR(WorkspaceUnchangedDocumentDiagnosticReport,
                resultId,  // Inherited from UnchangedDocumentDiagnosticReport
                uri,
                version
        )
);

GLAZE_META_NORMAL(WorkspaceDiagnosticReport,
        items
);

GLAZE_META_NORMAL(WorkspaceDiagnosticReportPartialResult,
        items
);

GLAZE_META_NORMAL(ParameterInformation,
        label,
        documentation
);

GLAZE_META_NORMAL(SignatureInformation,
        label,
        documentation,
        parameters,
        activeParameter
);

GLAZE_META_NORMAL(SignatureHelp,
        signatures,
        activeSignature,
        activeParameter
);

GLAZE_META_NORMAL(SignatureHelpContext,
        triggerKind,
        triggerCharacter,
        isRetrigger,
        activeSignatureHelp
);

GLAZE_META_NORMAL(SignatureHelpParams,
        textDocument,   // Inherited from TextDocumentPositionParams
        position,       // Inherited from TextDocumentPositionParams
        workDoneToken,  // Inherited from WorkDoneProgressParams
        context
);

GLAZE_META_NORMAL(CodeActionContext,
        diagnostics,
        only,
        triggerKind
);

GLAZE_META_NORMAL(CodeActionParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument,
        range,
        context
);

GLAZE_META_NORMAL(OptionalVersionedTextDocumentIdentifier,
        uri,  // Inherited from TextDocumentIdentifier
        version
);

GLAZE_META_NORMAL(AnnotatedTextEdit,
        range,    // Inherited from TextEdit
        newText,  // Inherited from TextEdit
        annotationId
);

GLAZE_META(StringValue,
        STATIC_FIELD(StringValue, kind),
        MAKE_TYPE_PAIR(StringValue,
                value
        )
);

GLAZE_META_NORMAL(SnippetTextEdit,
        range,
        snippet,
        annotationId
);

GLAZE_META_NORMAL(TextDocumentEdit,
        textDocument,
        edits
);

GLAZE_META_NORMAL(CreateFileOptions,
        overwrite,
        ignoreIfExists
);

GLAZE_META(CreateFile,
        STATIC_FIELD(CreateFile, kind),
        MAKE_TYPE_PAIR(CreateFile,
                uri,
                options,
                annotationId
        )
);

GLAZE_META_NORMAL(RenameFileOptions,
        overwrite,
        ignoreIfExists
);

GLAZE_META(RenameFile,
        STATIC_FIELD(RenameFile, kind),
        MAKE_TYPE_PAIR(RenameFile,
                oldUri,
                newUri,
                options,
                annotationId
        )
);

GLAZE_META_NORMAL(DeleteFileOptions,
        recursive,
        ignoreIfNotExists
);

GLAZE_META(DeleteFile,
        STATIC_FIELD(DeleteFile, kind),
        MAKE_TYPE_PAIR(DeleteFile,
                uri,
                options,
                annotationId
        )
);

GLAZE_META_NORMAL(ChangeAnnotation,
        label,
        needsConfirmation,
        description
);

GLAZE_META_NORMAL(WorkspaceEdit,
        changes,
        documentChanges,
        changeAnnotations
);

GLAZE_META_NORMAL(CodeAction::_Disabled,
        reason
);

GLAZE_META_NORMAL(CodeAction,
        title,
        kind,
        diagnostics,
        isPreferred,
        disabled,
        edit,
        command,
        data,
        tags
);

GLAZE_META_NORMAL(DocumentColorParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument
);

GLAZE_META_NORMAL(Color,
        red,
        green,
        blue,
        alpha
);

GLAZE_META_NORMAL(ColorInformation,
        range,
        color
);

GLAZE_META_NORMAL(ColorPresentationParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        textDocument,
        color,
        range
);

GLAZE_META_NORMAL(ColorPresentation,
        label,
        textEdit,
        additionalTextEdits
);

template<>
struct glz::meta<FormattingOptions> {
        static constexpr auto value = glz::object(MAKE_TYPE_PAIR(FormattingOptions,
                tabSize,
                insertSpaces,
                trimTrailingWhitespace,
                insertFinalNewline,
                trimFinalNewlines
        ));

        static constexpr auto unknown_read  = &FormattingOptions::extra;
        static constexpr auto unknown_write = &FormattingOptions::extra;
};

GLAZE_META_NORMAL(DocumentFormattingParams,
        workDoneToken, // Inherited from WorkDoneProgressParams
        textDocument,
        options
);

GLAZE_META_NORMAL(DocumentRangeFormattingParams,
        workDoneToken, // Inherited from WorkDoneProgressParams
        textDocument,
        range,
        options
);

GLAZE_META_NORMAL(DocumentRangesFormattingParams,
        workDoneToken, // Inherited from WorkDoneProgressParams
        textDocument,
        ranges,
        options
);

GLAZE_META_NORMAL(DocumentOnTypeFormattingParams,
        textDocument,
        position,
        ch,
        options
);

GLAZE_META_NORMAL(RenameParams,
        textDocument,   // Inherited from TextDocumentPositionParams
        position,       // Inherited from TextDocumentPositionParams
        workDoneToken,  // Inherited from WorkDoneProgressParams
        newName
);

GLAZE_META_NORMAL(PrepareRenameParams,
        textDocument,  // Inherited from TextDocumentPositionParams
        position,      // Inherited from TextDocumentPositionParams
        workDoneToken  // Inherited from WorkDoneProgressParams
);

GLAZE_META_NORMAL(_PrepareRenameRequestResult1,
        range,
        placeholder
);

GLAZE_META_NORMAL(_PrepareRenameRequestResult2,
        defaultBehavior
);

GLAZE_META_NORMAL(LinkedEditingRangeParams,
        textDocument,  // Inherited from TextDocumentPositionParams
        position,      // Inherited from TextDocumentPositionParams
        workDoneToken  // Inherited from WorkDoneProgressParams
);

GLAZE_META_NORMAL(LinkedEditingRanges,
        ranges,
        wordPattern
);

GLAZE_META_NORMAL(SelectedCompletionInfo,
        range,
        text
);

GLAZE_META_NORMAL(InlineCompletionContext,
        triggerKind,
        selectedCompletionInfo
);

GLAZE_META_NORMAL(InlineCompletionParams,
        textDocument,   // Inherited from TextDocumentPositionParams
        position,       // Inherited from TextDocumentPositionParams
        workDoneToken,  // Inherited from WorkDoneProgressParams
        context
);

GLAZE_META_NORMAL(InlineCompletionItem,
        insertText,
        filterText,
        range,
        command
);

GLAZE_META_NORMAL(InlineCompletionList,
        items
);

GLAZE_META_NORMAL(WorkspaceSymbolParams,
        workDoneToken,       // Inherited from WorkDoneProgressParams
        partialResultToken,  // Inherited from PartialResultParams
        query
);

GLAZE_META_NORMAL(WorkspaceSymbol,
        name,
        kind,
        tags,
        containerName,
        location,
        data
);

GLAZE_META_NORMAL(ConfigurationItem,
        scopeUri,
        section
);

GLAZE_META_NORMAL(ConfigurationParams,
        items
);

GLAZE_META_NORMAL(DidChangeConfigurationParams,
        settings
);

GLAZE_META_NORMAL(WorkspaceFoldersChangeEvent,
        added,
        removed
);

GLAZE_META_NORMAL(DidChangeWorkspaceFoldersParams,
        event
);

GLAZE_META_NORMAL(FileCreate,
        uri
);

GLAZE_META_NORMAL(CreateFilesParams,
        files
);

GLAZE_META_NORMAL(FileRename,
        oldUri,
        newUri
);

GLAZE_META_NORMAL(RenameFilesParams,
        files
);

GLAZE_META_NORMAL(FileDelete,
        uri
);

GLAZE_META_NORMAL(DeleteFilesParams,
        files
);

GLAZE_META_NORMAL(FileEvent,
        uri,
        type
);

GLAZE_META_NORMAL(DidChangeWatchedFilesParams,
        changes
);

GLAZE_META_NORMAL(ExecuteCommandParams,
        workDoneToken, // Inherited from WorkDoneProgressParams
        command,
        arguments
);

GLAZE_META_NORMAL(WorkspaceEditMetadata,
        isRefactoring
);

GLAZE_META_NORMAL(ApplyWorkspaceEditParams,
        label,
        edit,
        metadata
);

GLAZE_META_NORMAL(ApplyWorkspaceEditResult,
        applied,
        failureReason,
        failedChange
);

GLAZE_META_NORMAL(TextDocumentContentParams,
        uri
);

GLAZE_META_NORMAL(TextDocumentContentResult,
        text
);

GLAZE_META_NORMAL(TextDocumentContentRefreshParams,
        uri
);

GLAZE_META_NORMAL(ShowMessageParams,
        type,
        message
);

GLAZE_META_NORMAL(MessageActionItem,
        title
);

GLAZE_META_NORMAL(ShowMessageRequestParams,
        type,
        message,
        actions
);

GLAZE_META_NORMAL(ShowDocumentParams,
        uri,
        external,
        takeFocus,
        selection
);

GLAZE_META_NORMAL(ShowDocumentResult,
        success
);

GLAZE_META_NORMAL(LogMessageParams,
        type,
        message
);

GLAZE_META_NORMAL(WorkDoneProgressCreateParams,
        token
);

GLAZE_META_NORMAL(WorkDoneProgressCancelParams,
        token
);
