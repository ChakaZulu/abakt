object f_History: Tf_History
  Left = 386
  Top = 292
  Width = 560
  Height = 400
  Caption = 'History'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 560
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 328
    Width = 552
    Height = 39
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      552
      39)
    object bt_Ok: TButton
      Left = 463
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
  end
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 552
    Height = 328
    ActivePage = tab_ArchiveCatalogue
    Align = alClient
    TabIndex = 0
    TabOrder = 1
    object tab_ArchiveCatalogue: TTabSheet
      Caption = '&Archive Catalogue'
      ImageIndex = 1
      object lb: TListBox
        Left = 0
        Top = 165
        Width = 544
        Height = 135
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
      end
      object gbx_History: TGroupBox
        Left = 0
        Top = 0
        Width = 544
        Height = 138
        Align = alTop
        Caption = 'Settings'
        TabOrder = 1
        DesignSize = (
          544
          138)
        object Label3: TLabel
          Left = 10
          Top = 55
          Width = 165
          Height = 13
          Caption = '&Remove backups older than (days)'
          FocusControl = ed_MaxDays
        end
        object Label4: TLabel
          Left = 10
          Top = 75
          Width = 133
          Height = 13
          Caption = '&Keep a minimum of backups'
          FocusControl = ed_MinBackups
        end
        object Label5: TLabel
          Left = 10
          Top = 95
          Width = 136
          Height = 13
          Caption = 'Keep a &maximum of backups'
          FocusControl = ed_MaxBackups
        end
        object lbl_HistInfo_MinBackups: TLabel
          Left = 298
          Top = 75
          Width = 108
          Height = 13
          Caption = 'AUTO_TRANSLATED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
        end
        object lbl_HistInfo_MaxBackups: TLabel
          Left = 298
          Top = 95
          Width = 108
          Height = 13
          Caption = 'AUTO_TRANSLATED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
        end
        object lbl_HistInfo_MaxDays: TLabel
          Left = 298
          Top = 55
          Width = 108
          Height = 13
          Caption = 'AUTO_TRANSLATED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
        end
        object lbl_HistoryInfo: TLabel
          Left = 130
          Top = 15
          Width = 399
          Height = 30
          Anchors = [akLeft, akTop, akRight]
          AutoSize = False
          Caption = 'AUTO_TRANSLATED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = []
          ParentFont = False
          WordWrap = True
        end
        object ed_MaxDays: TEdit
          Left = 226
          Top = 52
          Width = 41
          Height = 21
          TabOrder = 1
          Text = '0'
          OnChange = ed_MaxDaysChange
          OnKeyPress = ed_History_KeyPress
        end
        object ud_MaxDays: TUpDown
          Left = 267
          Top = 52
          Width = 15
          Height = 21
          Associate = ed_MaxDays
          Min = 0
          Max = 32767
          Position = 0
          TabOrder = 2
          Thousands = False
          Wrap = False
        end
        object ed_MinBackups: TEdit
          Left = 226
          Top = 72
          Width = 41
          Height = 21
          TabOrder = 3
          Text = '0'
          OnChange = ed_MinBackupsChange
          OnKeyPress = ed_History_KeyPress
        end
        object ud_MinBackups: TUpDown
          Left = 267
          Top = 72
          Width = 15
          Height = 21
          Associate = ed_MinBackups
          Min = 0
          Max = 32767
          Position = 0
          TabOrder = 4
          Thousands = False
          Wrap = False
        end
        object ed_MaxBackups: TEdit
          Left = 226
          Top = 92
          Width = 41
          Height = 21
          TabOrder = 5
          Text = '0'
          OnChange = ed_MaxBackupsChange
          OnKeyPress = ed_History_KeyPress
        end
        object ud_MaxBackups: TUpDown
          Left = 267
          Top = 92
          Width = 15
          Height = 21
          Associate = ed_MaxBackups
          Min = 0
          Max = 32767
          Position = 0
          TabOrder = 6
          Thousands = False
          Wrap = False
        end
        object cb_UseRecycleBin: TCheckBox
          Left = 10
          Top = 115
          Width = 425
          Height = 17
          Caption = '&Use Recycle Bin if possible'
          TabOrder = 7
          OnClick = cb_UseRecycleBinClick
        end
        object cb_MaintainHistory: TCheckBox
          Left = 8
          Top = 16
          Width = 113
          Height = 17
          Caption = 'Ena&bled'
          Checked = True
          State = cbChecked
          TabOrder = 0
          OnClick = cb_MaintainHistoryClick
        end
      end
      object pnl_ToolBar: TPanel
        Left = 0
        Top = 138
        Width = 544
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        Caption = 'pnl_ToolBar'
        TabOrder = 2
        object tbar_ArchiveCatalogue: TToolBar
          Left = 0
          Top = 0
          Width = 544
          Height = 27
          Align = alClient
          ButtonHeight = 19
          ButtonWidth = 110
          EdgeBorders = [ebLeft, ebTop, ebRight, ebBottom]
          Indent = 8
          List = True
          ShowCaptions = True
          TabOrder = 0
          Wrapable = False
          object tb_ShowFileList: TToolButton
            Left = 8
            Top = 2
            AutoSize = True
            Caption = 'Show &List of Files'
            OnClick = tb_ShowFileListClick
          end
          object ToolButton3: TToolButton
            Left = 101
            Top = 2
            Width = 8
            Caption = 'ToolButton1'
            ImageIndex = 1
            Style = tbsSeparator
          end
          object tb_AddFiles: TToolButton
            Left = 109
            Top = 2
            AutoSize = True
            Caption = 'Add F&iles to List...'
            OnClick = bt_AddFilesClick
          end
          object ToolButton5: TToolButton
            Left = 203
            Top = 2
            Width = 8
            Caption = 'ToolButton2'
            ImageIndex = 2
            Style = tbsSeparator
          end
          object tb_ApplySettings: TToolButton
            Left = 211
            Top = 2
            AutoSize = True
            Caption = 'Apply Settings &Now'
            OnClick = tb_ApplySettingsClick
          end
        end
      end
    end
  end
  object odlg_HistoryFiles: TOpenDialog
    DefaultExt = 'zip'
    Filter = 'Backups (*.zip;*.7z)|*.zip;*.7z|All Files (*.*)|*.*'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing, ofDontAddToRecent]
    Title = 'Add Backup Files to History'
    Left = 168
    Top = 248
  end
end
