object f_Group: Tf_Group
  Left = 367
  Top = 259
  Width = 600
  Height = 397
  Caption = 'Group of Profiles'
  Color = clBtnFace
  Constraints.MinHeight = 397
  Constraints.MinWidth = 600
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 26
    Width = 592
    Height = 316
    ActivePage = tab_CustomConfiguration
    Align = alClient
    MultiLine = True
    TabHeight = 15
    TabIndex = 2
    TabOrder = 0
    TabPosition = tpBottom
    TabWidth = 40
    object tab_Profiles: TTabSheet
      ImageIndex = -1
      OnShow = tab_ProfilesShow
      object pnl_Descr_Profiles: TPanel
        Left = 0
        Top = 0
        Width = 584
        Height = 20
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 0
        object Label1: TLabel
          Left = 8
          Top = 3
          Width = 296
          Height = 13
          Caption = 'Select the Profiles to be executed in the order displayed below.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
        end
      end
      object tbar_Profiles: TToolBar
        Left = 0
        Top = 20
        Width = 584
        Height = 52
        AutoSize = True
        ButtonWidth = 73
        EdgeBorders = []
        EdgeInner = esNone
        EdgeOuter = esNone
        Flat = True
        Images = DM.imgs_Buttons
        List = True
        ParentShowHint = False
        ShowCaptions = True
        ShowHint = True
        TabOrder = 1
        object tb_ProfileAdd: TToolButton
          Left = 0
          Top = 0
          AutoSize = True
          Caption = '&Add'
          ImageIndex = 0
          OnClick = tb_ProfileAddClick
        end
        object tb_ProfileRemove: TToolButton
          Left = 50
          Top = 0
          AutoSize = True
          Caption = '&Remove'
          ImageIndex = 1
          OnClick = tb_ProfileRemoveClick
        end
        object tb_ProfileEdit: TToolButton
          Left = 121
          Top = 0
          AutoSize = True
          Caption = 'E&dit'
          ImageIndex = 2
          OnClick = tb_ProfileEditClick
        end
        object ToolButton4: TToolButton
          Left = 170
          Top = 0
          Width = 8
          Caption = 'ToolButton4'
          ImageIndex = 3
          Style = tbsSeparator
        end
        object tb_ProfileAll: TToolButton
          Left = 178
          Top = 0
          AutoSize = True
          Caption = 'A&ll'
          ImageIndex = 4
          OnClick = tb_ProfileAllClick
        end
        object tb_ProfileNone: TToolButton
          Left = 220
          Top = 0
          AutoSize = True
          Caption = '&None'
          ImageIndex = 5
          OnClick = tb_ProfileNoneClick
        end
        object ToolButton7: TToolButton
          Left = 0
          Top = 0
          Width = 8
          Caption = 'ToolButton7'
          ImageIndex = 6
          Wrap = True
          Style = tbsSeparator
        end
        object tb_ProfileUp: TToolButton
          Left = 0
          Top = 30
          Hint = 'Up'
          AutoSize = True
          Caption = 'Up'
          ImageIndex = 6
          OnClick = tb_ProfileUpClick
        end
        object tb_ProfileDown: TToolButton
          Left = 45
          Top = 30
          Hint = 'Down'
          AutoSize = True
          Caption = 'Down'
          ImageIndex = 7
          OnClick = tb_ProfileDownClick
        end
        object tb_ProfileSort: TToolButton
          Left = 104
          Top = 30
          AutoSize = True
          Caption = 'Sort'
          ImageIndex = 8
          OnClick = tb_ProfileSortClick
        end
        object ToolButton12: TToolButton
          Left = 154
          Top = 30
          Width = 8
          Caption = 'ToolButton12'
          ImageIndex = 4
          Style = tbsSeparator
        end
        object tb_ProfileClear: TToolButton
          Left = 162
          Top = 30
          AutoSize = True
          Caption = 'Clear'
          ImageIndex = 3
          OnClick = tb_ProfileClearClick
        end
      end
      object clb_Profiles: TCheckListBox
        Left = 0
        Top = 72
        Width = 584
        Height = 221
        OnClickCheck = SetGroupModified
        Align = alClient
        Columns = 1
        ItemHeight = 13
        TabOrder = 2
        OnKeyDown = clb_ProfilesKeyDown
      end
    end
    object tab_CustomFilterSets: TTabSheet
      ImageIndex = 2
      OnShow = tab_CustomFilterSetsShow
      object pnl_Descr_CustomFilterSets: TPanel
        Left = 0
        Top = 0
        Width = 584
        Height = 20
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 0
        object Label3: TLabel
          Left = 8
          Top = 3
          Width = 578
          Height = 13
          Caption = 
            'FilterSets that will be added to the FilterSet list in every exe' +
            'cuted Profile. They can also be used to adjust existing FilterSe' +
            'ts.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
        end
      end
      inline fr_CustomFilterSetList: TFrameFilterSetList
        Left = 0
        Top = 20
        Width = 584
        Height = 273
        Align = alClient
        TabOrder = 1
        inherited tbar: TToolBar
          Width = 584
          TabOrder = 0
        end
        inherited lv: TListView
          Width = 584
          Height = 221
          TabOrder = 1
        end
      end
    end
    object tab_CustomConfiguration: TTabSheet
      ImageIndex = -1
      OnShow = tab_CustomConfigurationShow
      object pnl_Descr_CustomConfiguration: TPanel
        Left = 0
        Top = 0
        Width = 584
        Height = 20
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 0
        object Label2: TLabel
          Left = 8
          Top = 3
          Width = 370
          Height = 13
          Caption = 
            'Select and modify the configuration settings to adjust in every ' +
            'executed Profile.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
        end
      end
      inline fr_CustomConfigEdit: TFrameConfigEdit
        Left = 0
        Top = 20
        Width = 584
        Height = 273
        Align = alClient
        TabOrder = 1
        inherited gbx_ProfileProperty: TGroupBox
          Top = 148
          Width = 584
          Font.Pitch = fpVariable
          inherited lbl_ItemCaption: TLabel
            Font.Pitch = fpVariable
          end
          inherited Panel5: TPanel
            Width = 580
            inherited pnl_ItemValue: TPanel
              Width = 572
            end
            inherited sbox_ObjectInfo: TScrollBox
              Width = 572
              inherited lbl_ObjectInfo: TLabel
                Font.Pitch = fpVariable
              end
            end
          end
          inherited Panel2: TPanel
            Width = 580
            DesignSize = (
              580
              22)
            inherited lbl_ItemListCaption: TLabel
              Font.Pitch = fpVariable
            end
            inherited ed_InternalNameValue: TEdit
              Width = 416
            end
          end
        end
        inherited Panel1: TPanel
          Width = 584
          Height = 148
          inherited tbar: TToolBar
            Width = 584
          end
          inherited lv: TListView
            Width = 584
            Height = 126
          end
        end
      end
    end
  end
  object pnl_ToolBars: TPanel
    Left = 0
    Top = 0
    Width = 592
    Height = 26
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object Panel4: TPanel
      Left = 0
      Top = 0
      Width = 93
      Height = 26
      Align = alLeft
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 0
      object ToolBar1: TToolBar
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
        object tb_FileNew: TToolButton
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
        object tb_FileSave: TToolButton
          Left = 61
          Top = 0
          Action = act_FileSave
          ImageIndex = 8
        end
        object ToolButton5: TToolButton
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
      Width = 385
      Height = 26
      Align = alClient
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 1
      object ToolBar2: TToolBar
        Left = 0
        Top = 0
        Width = 385
        Height = 26
        Align = alClient
        AutoSize = True
        ButtonWidth = 127
        Caption = 'tbar_ProfileTabs'
        EdgeBorders = []
        Flat = True
        Images = DM.imgs_Main
        List = True
        ShowCaptions = True
        TabOrder = 0
        object tb_ViewProfiles: TToolButton
          Left = 0
          Top = 0
          Action = act_EditProfiles
          AutoSize = True
          Grouped = True
          ImageIndex = 2
          Style = tbsCheck
        end
        object tb_ViewCustomFilterSets: TToolButton
          Left = 65
          Top = 0
          Action = act_EditCustomFilterSets
          AutoSize = True
          Grouped = True
          ImageIndex = 3
          Style = tbsCheck
        end
        object tb_ViewCustomConfiguration: TToolButton
          Left = 177
          Top = 0
          Action = act_EditCustomConfiguration
          AutoSize = True
          Grouped = True
          ImageIndex = 4
          Style = tbsCheck
        end
        object ToolButton1: TToolButton
          Left = 308
          Top = 0
          Width = 8
          Caption = 'ToolButton1'
          ImageIndex = 2
          Style = tbsSeparator
        end
      end
    end
    object pnl_ExecuteButton: TPanel
      Left = 478
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
        object ToolButton2: TToolButton
          Left = 0
          Top = 2
          Action = act_ExecuteGroup
          AutoSize = True
          ImageIndex = 1
        end
        object ToolButton3: TToolButton
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
    Images = DM.imgs_Main
    Left = 72
    Top = 120
    object mu_File: TMenuItem
      Caption = '&File'
      object mi_FileNew: TMenuItem
        Action = act_FileNew
      end
      object mi_FileOpen: TMenuItem
        Action = act_FileOpen
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object mi_FileSave: TMenuItem
        Action = act_FileSave
      end
      object SaveProfileGroupAs1: TMenuItem
        Action = act_FileSaveAs
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object mi_RecentFiles: TMenuItem
        Caption = '&Recent Files'
        ShortCut = 16466
        OnClick = mi_RecentFilesClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object mi_Close: TMenuItem
        Caption = '&Close'
        ShortCut = 32883
        OnClick = mi_CloseClick
      end
    end
    object mu_Edit: TMenuItem
      Caption = '&Edit'
      object N1Profiles1: TMenuItem
        Action = act_EditProfiles
      end
      object CustomFilterSets1: TMenuItem
        Action = act_EditCustomFilterSets
      end
      object N3BackupType1: TMenuItem
        Action = act_EditCustomConfiguration
      end
    end
    object mu_View: TMenuItem
      Caption = '&View'
      object BackupLog1: TMenuItem
        Action = act_ViewActionLog
      end
    end
    object mu_Tools: TMenuItem
      Caption = '&Tools'
      object mi_StartGroupBackup: TMenuItem
        Action = act_ExecuteGroup
      end
    end
    object mu_Help: TMenuItem
      Caption = '&Help'
      object Manual1: TMenuItem
        Action = act_Manual
      end
    end
  end
  object ActionList: TActionList
    Left = 152
    Top = 120
    object act_EditProfiles: TAction
      Category = 'Edit'
      Caption = 'Profiles'
      ShortCut = 117
      OnExecute = act_EditProfilesExecute
    end
    object act_EditCustomFilterSets: TAction
      Category = 'Edit'
      Caption = 'Custom FilterSets'
      ShortCut = 118
      OnExecute = act_EditCustomFilterSetsExecute
    end
    object act_EditCustomConfiguration: TAction
      Category = 'Edit'
      Caption = 'Custom Configuration'
      ShortCut = 119
      OnExecute = act_EditCustomConfigurationExecute
    end
    object act_FileNew: TAction
      Category = 'File'
      Caption = '&New Group'
      ShortCut = 16462
      OnExecute = act_FileNewExecute
    end
    object act_FileOpen: TAction
      Category = 'File'
      Caption = '&Open Group'
      ShortCut = 16463
      OnExecute = act_FileOpenExecute
    end
    object act_FileSave: TAction
      Category = 'File'
      Caption = '&Save Group'
      ShortCut = 16467
      OnExecute = act_FileSaveExecute
    end
    object act_ViewActionLog: TAction
      Category = 'View'
      Caption = 'Action &Log'
      ShortCut = 24664
      SecondaryShortCuts.Strings = (
        '')
      OnExecute = act_ViewActionLogExecute
    end
    object act_FileSaveAs: TAction
      Category = 'File'
      Caption = 'Save Group &As...'
      ShortCut = 24659
      OnExecute = act_FileSaveAsExecute
    end
    object act_ExecuteGroup: TAction
      Category = 'Tools'
      Caption = 'E&xecute Group'
      ShortCut = 49240
      SecondaryShortCuts.Strings = (
        'Ctrl+B')
      OnExecute = act_ExecuteGroupExecute
    end
    object act_Manual: TAction
      Category = 'Help'
      Caption = '&Manual'
      ShortCut = 112
      OnExecute = act_ManualExecute
    end
  end
  object odlg_Group: TOpenDialog
    DefaultExt = 'abg'
    Filter = 'Abakt Group (*.abg)|*.abg|All files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Open Group'
    Left = 152
    Top = 176
  end
  object sdlg_Group: TSaveDialog
    DefaultExt = 'abg'
    Filter = 'Abakt Group (*.abg)|*.abg|All files (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Save Group'
    Left = 72
    Top = 176
  end
  object odlg_Profiles: TOpenDialog
    DefaultExt = 'abp'
    Filter = 'Abakt Profile (*.abp)|*.abp|All files (*.*)|*.*'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Choose a Profile to add to the Group'
    Left = 232
    Top = 176
  end
end
