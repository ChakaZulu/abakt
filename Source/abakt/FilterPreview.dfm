object f_FilterPreview: Tf_FilterPreview
  Left = 405
  Top = 217
  Width = 500
  Height = 400
  Caption = 'Filter Preview'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 500
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 330
    Width = 492
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      492
      37)
    object bt_Close: TButton
      Left = 408
      Top = 7
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = '&Close'
      TabOrder = 0
      OnClick = bt_CloseClick
    end
    object bt_SaveActiveList: TButton
      Left = 8
      Top = 7
      Width = 273
      Height = 25
      Caption = '&Save list of selected files'
      TabOrder = 1
      OnClick = bt_SaveActiveListClick
    end
  end
  object pnl_Descr: TPanel
    Left = 0
    Top = 0
    Width = 492
    Height = 20
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 1
    object Label15: TLabel
      Left = 8
      Top = 3
      Width = 400
      Height = 13
      Caption = 
        'Show the files that will be included and excluded, according to ' +
        'the current FilterSets. '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
    end
  end
  object gbx_FileLists: TGroupBox
    Left = 0
    Top = 76
    Width = 492
    Height = 254
    Align = alClient
    Caption = 'FileLists'
    TabOrder = 2
    object PageControl: TPageControl
      Left = 2
      Top = 15
      Width = 488
      Height = 200
      Hint = 'Tip: Right-click mouse button for more features.'
      ActivePage = tab_ExcludedFiles
      Align = alClient
      Images = DM.imgs_Filters
      ParentShowHint = False
      ShowHint = True
      TabIndex = 1
      TabOrder = 0
      object tab_IncludedFiles: TTabSheet
        Caption = '&Included'
        ImageIndex = 1
        object lv_IncludeList: TListView
          Left = 0
          Top = 0
          Width = 480
          Height = 171
          Align = alClient
          Columns = <
            item
              Caption = 'Name'
              MinWidth = 50
              Width = 118
            end
            item
              Alignment = taRightJustify
              Caption = 'Size'
              MinWidth = 10
              Width = 60
            end
            item
              Caption = 'Attributes'
              MinWidth = 10
              Width = 42
            end
            item
              Caption = 'Modified'
              MinWidth = 10
              Width = 112
            end
            item
              Caption = 'Filter'
              MinWidth = 20
              Width = 128
            end>
          ColumnClick = False
          HideSelection = False
          MultiSelect = True
          ReadOnly = True
          PopupMenu = pop_Files
          SortType = stText
          TabOrder = 0
          ViewStyle = vsReport
        end
      end
      object tab_ExcludedFiles: TTabSheet
        Caption = '&Excluded'
        object lv_ExcludeList: TListView
          Left = 0
          Top = 0
          Width = 480
          Height = 171
          Align = alClient
          Columns = <
            item
              Caption = 'Name'
              MinWidth = 50
              Width = 118
            end
            item
              Alignment = taRightJustify
              Caption = 'Size'
              MinWidth = 10
              Width = 60
            end
            item
              Caption = 'Attributes'
              MinWidth = 10
              Width = 42
            end
            item
              Caption = 'Modified'
              MinWidth = 10
              Width = 112
            end
            item
              Caption = 'Filter'
              MinWidth = 20
              Width = 128
            end>
          ColumnClick = False
          HideSelection = False
          MultiSelect = True
          ReadOnly = True
          PopupMenu = pop_Files
          SortType = stText
          TabOrder = 0
          ViewStyle = vsReport
        end
      end
    end
    object pnl_Included: TPanel
      Left = 2
      Top = 215
      Width = 488
      Height = 18
      Align = alBottom
      BevelOuter = bvLowered
      TabOrder = 1
      object lbl_IncludedFilesInfo: TLabel
        Left = 126
        Top = 2
        Width = 96
        Height = 13
        Caption = 'lbl_IncludedFilesInfo'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 8
        Top = 2
        Width = 68
        Height = 13
        Caption = 'Included Files:'
      end
    end
    object pnl_Excluded: TPanel
      Left = 2
      Top = 233
      Width = 488
      Height = 19
      Align = alBottom
      BevelOuter = bvLowered
      TabOrder = 2
      object Label4: TLabel
        Left = 8
        Top = 2
        Width = 71
        Height = 13
        Caption = 'Excluded Files:'
      end
      object lbl_ExcludedFilesInfo: TLabel
        Left = 126
        Top = 2
        Width = 99
        Height = 13
        Caption = 'lbl_ExcludedFilesInfo'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
      end
    end
  end
  object gbx_BuildFileLists: TGroupBox
    Left = 0
    Top = 20
    Width = 492
    Height = 56
    Align = alTop
    Caption = 'Build FileLists'
    TabOrder = 3
    DesignSize = (
      492
      56)
    object bt_Build: TButton
      Left = 8
      Top = 18
      Width = 129
      Height = 25
      Caption = '&Build'
      Default = True
      TabOrder = 0
      OnClick = bt_BuildClick
    end
    object cb_ScanFullTree: TCheckBox
      Left = 152
      Top = 33
      Width = 241
      Height = 17
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Scan exluded directories'
      TabOrder = 1
    end
    object cb_StripDrive: TCheckBox
      Left = 152
      Top = 15
      Width = 137
      Height = 17
      Caption = 'Strip &drive'
      TabOrder = 2
    end
    object cb_AddQuotes: TCheckBox
      Left = 304
      Top = 15
      Width = 89
      Height = 17
      Anchors = [akLeft, akTop, akRight]
      Caption = '&Quotes'
      TabOrder = 3
    end
    object bt_Abort: TButton
      Left = 408
      Top = 16
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '&Abort'
      Enabled = False
      TabOrder = 4
      OnClick = bt_AbortClick
    end
  end
  object sdlg_FileList: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'Text File (*.txt)|*.txt|Any File (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Save File List'
    Left = 152
    Top = 144
  end
  object pop_Files: TPopupMenu
    Left = 64
    Top = 144
    object pmi_ClearFileList: TMenuItem
      Caption = '&Clear List'
      OnClick = pmi_ClearFileListClick
    end
    object pmi_SelectAllFiles: TMenuItem
      Caption = 'Select &All'
      ShortCut = 16449
      OnClick = pmi_SelectAllFilesClick
    end
  end
end
