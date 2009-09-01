object f_TemplateEditor: Tf_TemplateEditor
  Left = 359
  Top = 237
  Width = 500
  Height = 400
  Caption = 'Template Editor'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 500
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poDesktopCenter
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
    Width = 492
    Height = 319
    ActivePage = tab_CustomFilterSets
    Align = alClient
    MultiLine = True
    TabHeight = 15
    TabIndex = 0
    TabOrder = 0
    TabPosition = tpBottom
    object tab_CustomFilterSets: TTabSheet
      ImageIndex = 1
      OnShow = tab_CustomFilterSetsShow
      inline fr_CustomFilterSetList: TFrameFilterSetList
        Left = 0
        Top = 0
        Width = 484
        Height = 296
        Align = alClient
        TabOrder = 0
        inherited tbar: TToolBar
          Width = 484
          TabOrder = 0
        end
        inherited lv: TListView
          Width = 484
          Height = 244
          TabOrder = 1
        end
      end
    end
    object tab_CustomConfiguration: TTabSheet
      OnShow = tab_CustomConfigurationShow
      object pnl_Descr_CustomConfiguration: TPanel
        Left = 0
        Top = 0
        Width = 484
        Height = 20
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 0
        object Label15: TLabel
          Left = 8
          Top = 3
          Width = 314
          Height = 13
          Caption = 
            'Only the selected (checked) settings will be written to the temp' +
            'late.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clMaroon
          Font.Height = -12
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
        end
      end
      inline fr_CustomConfigEdit: TFrameConfigEdit
        Left = 0
        Top = 20
        Width = 484
        Height = 276
        Align = alClient
        TabOrder = 1
        inherited gbx_ProfileProperty: TGroupBox
          Top = 151
          Width = 484
          Font.Pitch = fpVariable
          inherited lbl_ItemCaption: TLabel
            Font.Pitch = fpVariable
          end
          inherited Panel5: TPanel
            Width = 480
            inherited pnl_ItemValue: TPanel
              Width = 472
            end
            inherited sbox_ObjectInfo: TScrollBox
              Width = 472
              inherited lbl_ObjectInfo: TLabel
                Font.Pitch = fpVariable
              end
            end
          end
          inherited Panel2: TPanel
            Width = 480
            DesignSize = (
              480
              22)
            inherited lbl_ItemListCaption: TLabel
              Font.Pitch = fpVariable
            end
            inherited ed_InternalNameValue: TEdit
              Width = 316
            end
          end
        end
        inherited Panel1: TPanel
          Width = 484
          Height = 151
          inherited tbar: TToolBar
            Width = 484
          end
          inherited lv: TListView
            Width = 484
            Height = 129
          end
        end
      end
    end
  end
  object pnl_Toolbars: TPanel
    Left = 0
    Top = 0
    Width = 492
    Height = 26
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object Panel3: TPanel
      Left = 484
      Top = 0
      Width = 8
      Height = 26
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
    end
    object pnl_FileToolBar: TPanel
      Left = 0
      Top = 0
      Width = 93
      Height = 26
      Align = alLeft
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 1
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
        object tb_FileNew: TToolButton
          Left = 0
          Top = 0
          ImageIndex = 6
          OnClick = mi_FileNewClick
        end
        object tb_FileOpen: TToolButton
          Left = 23
          Top = 0
          ImageIndex = 7
          Style = tbsDropDown
          OnClick = mi_FileOpenClick
        end
        object tb_FileSave: TToolButton
          Left = 61
          Top = 0
          ImageIndex = 8
          OnClick = mi_FileSaveClick
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
      Width = 391
      Height = 26
      Align = alClient
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 2
      object tbar_ProfileTabs: TToolBar
        Left = 0
        Top = 0
        Width = 262
        Height = 26
        Align = alLeft
        AutoSize = True
        ButtonWidth = 127
        Caption = 'tbar_ProfileTabs'
        EdgeBorders = []
        Flat = True
        Images = DM.imgs_Main
        List = True
        ShowCaptions = True
        TabOrder = 0
        object tb_ViewCustomFilterSets: TToolButton
          Left = 0
          Top = 0
          Action = act_ViewPageCustomFilterSets
          AutoSize = True
          Grouped = True
          ImageIndex = 3
          Style = tbsCheck
        end
        object tb_ViewCustomConfiguration: TToolButton
          Left = 112
          Top = 0
          Action = act_ViewPageCustomConfiguration
          AutoSize = True
          Grouped = True
          ImageIndex = 4
          Style = tbsCheck
        end
        object ToolButton2: TToolButton
          Left = 243
          Top = 0
          Width = 8
          Caption = 'ToolButton2'
          ImageIndex = 6
          Style = tbsSeparator
        end
      end
    end
  end
  object MainMenu: TMainMenu
    Left = 192
    Top = 144
    object mu_File: TMenuItem
      Caption = '&File'
      object mi_FileNew: TMenuItem
        Caption = '&New Template'
        ShortCut = 16462
        OnClick = mi_FileNewClick
      end
      object mi_FileOpen: TMenuItem
        Caption = '&Open Template'
        ShortCut = 16463
        OnClick = mi_FileOpenClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object mi_FileSave: TMenuItem
        Caption = '&Save Template'
        ShortCut = 16467
        OnClick = mi_FileSaveClick
      end
      object mi_FileSaveAs: TMenuItem
        Caption = 'Save Template &As...'
        ShortCut = 24659
        OnClick = mi_FileSaveAsClick
      end
      object N3: TMenuItem
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
      object mi_FileClose: TMenuItem
        Caption = '&Close'
        ShortCut = 32883
        OnClick = mi_CloseClick
      end
    end
    object mu_View: TMenuItem
      Caption = '&View'
      object mi_ViewPageCustomFilterSets: TMenuItem
        Action = act_ViewPageCustomFilterSets
      end
      object mi_ViewPageCustomConfiguration: TMenuItem
        Action = act_ViewPageCustomConfiguration
      end
    end
    object mu_Tools: TMenuItem
      Caption = '&Tools'
      object mi_ToolsPopulateWithCurrentProfile: TMenuItem
        Caption = 'Current Profile --> This Template'
        OnClick = mi_ToolsPopulateWithCurrentProfileClick
      end
      object mi_ToolsApplyToCurrentProfile: TMenuItem
        Caption = 'This Template --> Current Profile'
        OnClick = mi_ToolsApplyToCurrentProfileClick
      end
    end
  end
  object odlg_Template: TOpenDialog
    DefaultExt = 'abt'
    Filter = 
      'Abakt Template (*.abt)|*.abt|Import Abakt Profile or Group (*.ab' +
      'p;*.abg)|*.abp;*.abg|All files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing, ofDontAddToRecent, ofForceShowHidden]
    Title = 'Open/Import Template'
    Left = 328
    Top = 144
  end
  object sdlg_Template: TSaveDialog
    DefaultExt = 'abt'
    Filter = 'Abakt Template (*.abt)|*.abt|All files (*.*)|*.*'
    Options = [ofOverwritePrompt, ofPathMustExist, ofEnableSizing, ofDontAddToRecent, ofForceShowHidden]
    Left = 408
    Top = 144
  end
  object ActionList: TActionList
    Left = 256
    Top = 144
    object act_ViewPageCustomFilterSets: TAction
      Category = 'View'
      Caption = 'Custom FilterSets'
      ShortCut = 118
      OnExecute = act_ViewPageCustomFilterSetsExecute
    end
    object act_ViewPageCustomConfiguration: TAction
      Category = 'View'
      Caption = 'Custom Configuration'
      ShortCut = 119
      OnExecute = act_ViewPageCustomConfigurationExecute
    end
  end
end
