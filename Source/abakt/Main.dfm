object f_Main: Tf_Main
  Left = 368
  Top = 205
  Width = 601
  Height = 446
  Caption = 'Abakt'
  Color = clBtnFace
  Constraints.MinHeight = 446
  Constraints.MinWidth = 601
  DefaultMonitor = dmPrimary
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  ShowHint = True
  OnCanResize = FormCanResize
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter: TSplitter
    Left = 0
    Top = 302
    Width = 593
    Height = 6
    Cursor = crVSplit
    Align = alBottom
    AutoSnap = False
    Beveled = True
    MinSize = 36
    ResizeStyle = rsUpdate
    OnCanResize = SplitterCanResize
  end
  object PageControl: TPageControl
    Left = 0
    Top = 26
    Width = 593
    Height = 276
    ActivePage = tab_Source
    Align = alClient
    TabHeight = 15
    TabIndex = 0
    TabOrder = 0
    TabPosition = tpBottom
    TabWidth = 40
    object tab_Source: TTabSheet
      ImageIndex = -1
      OnShow = tab_SourceShow
      inline fr_Source: TFrameSource
        Left = 0
        Top = 0
        Width = 585
        Height = 253
        Align = alClient
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        inherited Splitter: TSplitter
          Height = 233
        end
        inherited gbx_Files: TGroupBox
          Width = 337
          Height = 233
          inherited lv_Files: TListView
            Width = 333
            Height = 176
          end
          inherited pnl_FilesSummary: TPanel
            Top = 213
            Width = 333
          end
          inherited pnl_FileToolBars: TPanel
            Width = 333
            inherited tbr_FilterIcons: TToolBar
              Width = 233
            end
          end
        end
        inherited gbx_Dirs: TGroupBox
          Height = 233
          inherited tv_Dirs: TTreeView
            Height = 194
          end
        end
        inherited pnl_Descr_Source: TPanel
          Width = 585
        end
      end
    end
    object tab_Filters: TTabSheet
      ImageIndex = -1
      OnShow = tab_FiltersShow
      inline fr_Filters: TFrameFilters
        Left = 0
        Top = 0
        Width = 585
        Height = 253
        Align = alClient
        TabOrder = 0
        inherited pnl_Descr_Filters: TPanel
          Width = 585
          inherited Label4: TLabel
            Font.Pitch = fpVariable
          end
        end
        inherited gbx_FilterSetList: TGroupBox
          Width = 585
          Height = 233
          inherited fr_FilterSetList: TFrameFilterSetList
            Width = 581
            Height = 216
            inherited tbar: TToolBar
              Width = 581
            end
            inherited lv: TListView
              Width = 581
              Height = 164
            end
          end
        end
      end
    end
    object tab_Action: TTabSheet
      ImageIndex = 4
      OnShow = tab_ActionShow
      object pgc_Action: TPageControl
        Left = 0
        Top = 0
        Width = 585
        Height = 253
        ActivePage = tab_ActionType
        Align = alClient
        TabIndex = 0
        TabOrder = 0
        object tab_ActionType: TTabSheet
          Caption = 'F8 - Action Type'
          inline fr_ActionType: TFrameActionType
            Left = 0
            Top = 0
            Width = 577
            Height = 225
            Align = alClient
            AutoScroll = False
            TabOrder = 0
            inherited ScrollBox_ActionType: TScrollBox
              Width = 577
              Height = 225
              inherited pnl_Descr_ActionType: TPanel
                Width = 555
                inherited Label15: TLabel
                  Font.Pitch = fpVariable
                end
              end
              inherited gbx_ActionType: TGroupBox
                Width = 555
                inherited Label1: TLabel
                  Font.Pitch = fpVariable
                end
                inherited Label2: TLabel
                  Font.Pitch = fpVariable
                end
                inherited rb_File_Delete: TRadioButton
                  Font.Pitch = fpVariable
                end
              end
              inherited gbx_ArchiveMode: TGroupBox
                Width = 555
                inherited Label16: TLabel
                  Font.Pitch = fpVariable
                end
                inherited Label29: TLabel
                  Font.Pitch = fpVariable
                end
                inherited Label32: TLabel
                  Font.Pitch = fpVariable
                end
                inherited Label33: TLabel
                  Font.Pitch = fpVariable
                end
                inherited Label34: TLabel
                  Font.Pitch = fpVariable
                end
                inherited lbl_ArchiveModeInfo: TLabel
                  Font.Pitch = fpVariable
                end
                inherited Label3: TLabel
                  Font.Pitch = fpVariable
                end
              end
              inherited Panel2: TPanel
                Width = 555
                inherited cb_IncludeEmptySourceDirectories: TCheckBox
                  Width = 532
                end
              end
            end
          end
        end
        object tab_ActionTypeSettings: TTabSheet
          Caption = 'F9 - Action Type Settings'
          ImageIndex = -1
          OnShow = tab_ActionTypeSettingsShow
          inline fr_ActionTypeSettings: TFrameActionTypeSettings
            Left = 0
            Top = 0
            Width = 577
            Height = 225
            Align = alClient
            AutoScroll = False
            TabOrder = 0
            inherited pgc_ActionTypes: TPageControl
              Width = 577
              Height = 205
              inherited tab_File: TTabSheet
                inherited pnl_Descr_actFile: TPanel
                  Width = 569
                end
                inherited Panel4: TPanel
                  Width = 569
                  inherited lbl_FileInfo_PathInformation: TLabel
                    Width = 312
                  end
                  inherited cb_File_Overwrite: TCheckBox
                    Width = 550
                  end
                  inherited cb_File_UseRecycleBin: TCheckBox
                    Width = 550
                  end
                  inherited cb_File_OverwriteReadOnly: TCheckBox
                    Width = 530
                  end
                end
              end
            end
            inherited pnl_Descr_ActSets: TPanel
              Width = 577
            end
          end
        end
        object tab_Destination: TTabSheet
          Caption = 'F10 - Destination'
          ImageIndex = 1
          OnShow = tab_DestinationShow
          inline fr_Destination: TFrameDestination
            Left = 0
            Top = 0
            Width = 577
            Height = 225
            Align = alClient
            AutoScroll = False
            TabOrder = 0
            inherited ScrollBox_Dest: TScrollBox
              Width = 577
              Height = 225
              inherited gbx_Name: TGroupBox
                Width = 573
                inherited ed_NameTemplate: TEdit
                  Width = 437
                end
                inherited ed_FormattedNamePreview: TEdit
                  Width = 437
                end
                inherited cb_AppendExtension: TCheckBox
                  Width = 548
                end
                inherited cmbx_NameTemplateExamples: TComboBox
                  Width = 437
                end
              end
              inherited pnl_Descr_Destination: TPanel
                Width = 573
              end
              inherited gbx_Directory: TGroupBox
                Width = 573
                inherited ed_Directory: TEdit
                  Width = 437
                end
                inherited cb_AutoCreateDir: TCheckBox
                  Width = 548
                end
              end
            end
          end
        end
      end
    end
    object tab_Finalize: TTabSheet
      ImageIndex = -1
      OnShow = tab_FinalizeShow
      inline fr_Finalize: TFrameFinalize
        Left = 0
        Top = 0
        Width = 585
        Height = 259
        Align = alClient
        TabOrder = 0
        inherited ScrollBox_Finalize: TScrollBox
          Width = 585
          Height = 259
          inherited Panel1: TPanel
            Width = 581
          end
          inherited Panel10: TPanel
            Width = 581
          end
          inherited gbx_History: TGroupBox
            Width = 581
          end
          inherited pnl_Descr_Finalize: TPanel
            Width = 581
          end
          inherited gbx_FileList: TGroupBox
            Width = 581
          end
        end
      end
    end
  end
  object gbx_Status: TGroupBox
    Left = 0
    Top = 308
    Width = 593
    Height = 83
    Align = alBottom
    Caption = 'Status'
    Constraints.MinHeight = 36
    TabOrder = 1
    object lb_Status: TListBox
      Left = 2
      Top = 15
      Width = 589
      Height = 66
      Style = lbOwnerDrawFixed
      AutoComplete = False
      Align = alClient
      ExtendedSelect = False
      ItemHeight = 13
      PopupMenu = pop_Status
      TabOrder = 0
      TabWidth = 8
      OnDrawItem = lb_StatusDrawItem
    end
  end
  object pnl_Toolbars: TPanel
    Left = 0
    Top = 0
    Width = 593
    Height = 26
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    object pnl_FileToolBar: TPanel
      Left = 0
      Top = 0
      Width = 93
      Height = 26
      Align = alLeft
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 0
      object tbar_File: TToolBar
        Left = 0
        Top = 0
        Width = 93
        Height = 26
        Align = alClient
        AutoSize = True
        Caption = 'tbar_File'
        EdgeBorders = []
        Flat = True
        Images = DM.imgs_Main
        TabOrder = 0
        Wrapable = False
        object tb_ProfileNew: TToolButton
          Left = 0
          Top = 0
          Action = act_FileNew
          ImageIndex = 6
        end
        object tb_FileOpen: TToolButton
          Left = 23
          Top = 0
          Action = act_FileOpen
          ImageIndex = 7
          Style = tbsDropDown
        end
        object tb_ProfileSave: TToolButton
          Left = 61
          Top = 0
          Action = act_FileSave
          ImageIndex = 8
        end
        object ToolButton1: TToolButton
          Left = 84
          Top = 0
          Width = 8
          Caption = 'ToolButton1'
          ImageIndex = 9
          Style = tbsSeparator
        end
      end
    end
    object pnl_TabsToolbar: TPanel
      Left = 93
      Top = 0
      Width = 386
      Height = 26
      Align = alClient
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 1
      object tbar_ProfileTabs: TToolBar
        Left = 0
        Top = 0
        Width = 386
        Height = 26
        Align = alClient
        AutoSize = True
        ButtonWidth = 83
        Caption = 'tbar_ProfileTabs'
        EdgeBorders = []
        Flat = True
        Images = DM.imgs_Main
        List = True
        ShowCaptions = True
        TabOrder = 0
        object tb_Source: TToolButton
          Left = 0
          Top = 0
          Action = act_EditSource
          AutoSize = True
          Grouped = True
          ImageIndex = 2
          Style = tbsCheck
        end
        object tb_Filters: TToolButton
          Left = 80
          Top = 0
          Action = act_EditFilters
          AutoSize = True
          Grouped = True
          ImageIndex = 3
          Style = tbsCheck
        end
        object tb_Action: TToolButton
          Left = 153
          Top = 0
          Action = act_EditAction
          AutoSize = True
          Grouped = True
          ImageIndex = 4
          Style = tbsCheck
        end
        object tb_Finalize: TToolButton
          Left = 229
          Top = 0
          Action = act_EditFinalize
          AutoSize = True
          Grouped = True
          ImageIndex = 5
          Style = tbsCheck
        end
        object ToolButton2: TToolButton
          Left = 310
          Top = 0
          Width = 20
          Caption = 'ToolButton2'
          ImageIndex = 6
          Style = tbsSeparator
        end
      end
    end
    object pnl_ExecuteButton: TPanel
      Left = 479
      Top = 0
      Width = 114
      Height = 26
      Align = alRight
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 2
      object tbar_ExecuteButton: TToolBar
        Left = -2
        Top = 0
        Width = 114
        Height = 26
        Align = alRight
        AutoSize = True
        ButtonWidth = 104
        EdgeBorders = []
        EdgeInner = esLowered
        EdgeOuter = esRaised
        Images = DM.imgs_Main
        List = True
        ShowCaptions = True
        TabOrder = 0
        object tb_ExecuteProfile: TToolButton
          Left = 0
          Top = 2
          Action = act_ExecuteProfile
          AutoSize = True
          ImageIndex = 0
        end
        object ToolButton4: TToolButton
          Left = 102
          Top = 2
          Width = 12
          Caption = 'ToolButton3'
          ImageIndex = 1
          Style = tbsSeparator
        end
      end
    end
  end
  object MainMenu: TMainMenu
    Left = 272
    Top = 200
    object mu_File: TMenuItem
      AutoHotkeys = maAutomatic
      Caption = '&File'
      object mi_FileNew: TMenuItem
        Action = act_FileNew
      end
      object mi_FileOpen: TMenuItem
        Action = act_FileOpen
      end
      object mi_FileClose: TMenuItem
        Action = act_FileClose
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object mi_FileSave: TMenuItem
        Action = act_FileSave
      end
      object mi_FileSaveAs: TMenuItem
        Action = act_FileSaveAs
      end
      object N8: TMenuItem
        Caption = '-'
      end
      object mi_StatusSaveAs: TMenuItem
        Action = act_StatusSaveAs
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object mi_RecentFiles: TMenuItem
        Caption = '&Recent Files'
        ShortCut = 16466
        OnClick = mi_RecentFilesClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object mi_FileExit: TMenuItem
        Action = act_Exit
      end
    end
    object mu_Edit: TMenuItem
      Caption = '&Edit'
      object mi_ViewSource: TMenuItem
        Action = act_EditSource
      end
      object mi_ViewFilters: TMenuItem
        Action = act_EditFilters
      end
      object mi_ViewAction: TMenuItem
        Action = act_EditAction
      end
      object mi_ViewActionType: TMenuItem
        Action = act_EditActionType
      end
      object mi_ViewActionTypeSettings: TMenuItem
        Action = act_EditActionTypeSettings
      end
      object mi_ViewDestination: TMenuItem
        Action = act_EditDestination
      end
      object mi_ViewFinalize: TMenuItem
        Action = act_EditFinalize
      end
      object ManageHistory1: TMenuItem
        Action = act_EditHistory
      end
      object N10: TMenuItem
        Caption = '-'
      end
      object mi_Preferences: TMenuItem
        Action = act_Preferences
      end
    end
    object mu_View: TMenuItem
      Caption = '&View'
      OnClick = mu_ViewClick
      object mi_ViewActionLog: TMenuItem
        Action = act_ViewActionLog
      end
      object N14: TMenuItem
        Caption = '-'
      end
      object mi_Source_RefreshDirectoryList: TMenuItem
        Caption = '&Refresh Directory List'
        ShortCut = 116
        OnClick = mi_Source_RefreshDirectoryListClick
      end
      object mi_Source_RefreshFileList: TMenuItem
        Caption = 'R&efresh File List'
        ShortCut = 8308
        OnClick = mi_Source_RefreshFileListClick
      end
      object mi_Source_ScanTreeBelowDir: TMenuItem
        Caption = 'Sca&n Tree Below Directory'
        ShortCut = 16500
        OnClick = mi_Source_ScanTreeBelowDirClick
      end
      object N12: TMenuItem
        Caption = '-'
      end
      object actClearStatus1: TMenuItem
        Action = act_ClearStatus
      end
    end
    object mu_Tools: TMenuItem
      Caption = '&Tools'
      object mi_ToolsExecuteProfile: TMenuItem
        Action = act_ExecuteProfile
      end
      object mi_ToolsFilterPreview: TMenuItem
        Action = act_FilterPreview
      end
      object N13: TMenuItem
        Caption = '-'
      end
      object mi_ToolsRestoreZipArchive: TMenuItem
        Action = act_RestoreZipArchive
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object mi_ToolsGroup: TMenuItem
        Action = act_Group
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object mi_ToolsTemplateEditor: TMenuItem
        Action = act_TemplateEditor
      end
    end
    object mu_Help: TMenuItem
      Caption = '&Help'
      object mi_Help: TMenuItem
        Action = act_Manual
      end
      object mi_SelectManual: TMenuItem
        Caption = '&Select Manual...'
        OnClick = mi_SelectManualClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object mi_ShowDescriptionBoxes: TMenuItem
        Action = act_ToggleDescriptionBoxes
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object mi_AbaktWebsite: TMenuItem
        Caption = 'Abakt &Homepage'
        OnClick = mi_AbaktWebsiteClick
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object mi_About: TMenuItem
        Action = act_About
      end
    end
  end
  object ActionList: TActionList
    Left = 336
    Top = 200
    object act_FileNew: TAction
      Category = 'File'
      Caption = '&New Profile'
      ShortCut = 16462
      OnExecute = act_FileNewExecute
    end
    object act_EditSource: TAction
      Category = 'Edit'
      Caption = '&1 - Source'
      ShortCut = 117
      OnExecute = act_EditSourceExecute
    end
    object act_EditFilters: TAction
      Category = 'Edit'
      Caption = '&2 - Filters'
      ShortCut = 118
      OnExecute = act_EditFiltersExecute
    end
    object act_EditAction: TAction
      Category = 'Edit'
      Caption = '&3 - Action'
      OnExecute = act_EditActionExecute
    end
    object act_FileOpen: TAction
      Category = 'File'
      Caption = '&Open Profile...'
      ShortCut = 16463
      OnExecute = act_FileOpenExecute
    end
    object act_FileSave: TAction
      Category = 'File'
      Caption = '&Save Profile'
      ShortCut = 16467
      OnExecute = act_FileSaveExecute
    end
    object act_FileSaveAs: TAction
      Category = 'File'
      Caption = 'Save Profile &As...'
      ShortCut = 24659
      OnExecute = act_FileSaveAsExecute
    end
    object act_ExecuteProfile: TAction
      Category = 'Tools'
      Caption = 'E&xecute Profile'
      ShortCut = 49240
      OnExecute = act_ExecuteProfileExecute
    end
    object act_Group: TAction
      Category = 'Tools'
      Caption = '&Group of Profiles...'
      ShortCut = 16455
      OnExecute = act_GroupExecute
    end
    object act_FilterPreview: TAction
      Category = 'Tools'
      Caption = '&Filter Preview...'
      ShortCut = 24646
      OnExecute = act_FilterPreviewExecute
    end
    object act_FileClose: TAction
      Category = 'File'
      Caption = '&Close Profile'
      ShortCut = 16471
      OnExecute = act_FileCloseExecute
    end
    object act_TemplateEditor: TAction
      Category = 'Tools'
      Caption = '&Template Editor...'
      ShortCut = 16468
      OnExecute = act_TemplateEditorExecute
    end
    object act_Manual: TAction
      Category = 'Help'
      Caption = '&Manual'
      ShortCut = 112
      OnExecute = act_ManualExecute
    end
    object act_About: TAction
      Category = 'Help'
      Caption = '&About'
      ShortCut = 16496
      OnExecute = act_AboutExecute
    end
    object act_RestoreZipArchive: TAction
      Category = 'Tools'
      Caption = '&Restore Zip Archive...'
      ShortCut = 24658
      OnExecute = act_RestoreZipArchiveExecute
    end
    object act_EditActionType: TAction
      Category = 'Edit'
      Caption = '3&a - Action Type'
      ShortCut = 119
      OnExecute = act_EditActionTypeExecute
    end
    object act_StatusSaveAs: TAction
      Category = 'File'
      Caption = 'Save S&tatus As...'
      ShortCut = 49235
      OnExecute = act_StatusSaveAsExecute
    end
    object act_Exit: TAction
      Category = 'File'
      Caption = 'E&xit'
      OnExecute = act_ExitExecute
    end
    object act_EditActionTypeSettings: TAction
      Category = 'Edit'
      Caption = '3&b - Action Type Settings'
      ShortCut = 120
      OnExecute = act_EditActionTypeSettingsExecute
    end
    object act_EditDestination: TAction
      Category = 'Edit'
      Caption = '3&c - Destination'
      ShortCut = 121
      OnExecute = act_EditDestinationExecute
    end
    object act_EditFinalize: TAction
      Category = 'Edit'
      Caption = '&4 - Finalize'
      ShortCut = 122
      OnExecute = act_EditFinalizeExecute
    end
    object act_ViewActionLog: TAction
      Category = 'View'
      Caption = 'Action &Log'
      ShortCut = 24664
      OnExecute = act_ViewActionLogExecute
    end
    object act_ClearStatus: TAction
      Category = 'View'
      Caption = 'Clear &Status Window'
      OnExecute = act_ClearStatusExecute
    end
    object act_EditHistory: TAction
      Category = 'Edit'
      Caption = '4a - &History...'
      ShortCut = 16456
      OnExecute = act_EditHistoryExecute
    end
    object act_Preferences: TAction
      Category = 'Edit'
      Caption = '&Preferences...'
      ShortCut = 16464
      OnExecute = act_PreferencesExecute
    end
    object act_ToggleDescriptionBoxes: TAction
      Category = 'Help'
      Caption = 'Show &Description Boxes'
      Checked = True
      ShortCut = 24688
      OnExecute = act_ToggleDescriptionBoxesExecute
    end
  end
  object sdlg_Profile: TSaveDialog
    DefaultExt = 'abp'
    Filter = 'Abakt Profile (*.abp)|*.abp|All files (*.*)|*.*'
    Options = [ofOverwritePrompt, ofPathMustExist, ofEnableSizing]
    Title = 'Save Profile'
    Left = 456
    Top = 200
  end
  object odlg_Profile: TOpenDialog
    DefaultExt = 'abp'
    Filter = 'Abakt Profile (*.abp)|*.abp|All files (*.*)|*.*'
    Options = [ofPathMustExist, ofFileMustExist, ofEnableSizing, ofForceShowHidden]
    Title = 'Open Profile'
    Left = 392
    Top = 200
  end
  object sdlg_Status: TSaveDialog
    DefaultExt = 'log'
    Filter = 
      'Log files (*.log)|*.log|Text files (*.txt)|*.txt|All files (*.*)' +
      '|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Save Status As...'
    Left = 480
    Top = 288
  end
  object pop_Status: TPopupMenu
    Left = 520
    Top = 200
    object ClearStatus1: TMenuItem
      Action = act_ClearStatus
    end
    object SaveStatusAs2: TMenuItem
      Action = act_StatusSaveAs
    end
  end
  object odlg_Manual: TOpenDialog
    DefaultExt = 'chm'
    Filter = 'Compiled HTML Help (*.chm)|*.chm|All files (*.*)|*.*'
    FilterIndex = 0
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Select Manual'
    Left = 384
    Top = 72
  end
end
