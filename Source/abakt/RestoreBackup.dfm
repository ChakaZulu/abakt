object f_RestoreBackup: Tf_RestoreBackup
  Left = 349
  Top = 197
  Width = 580
  Height = 538
  Caption = 'Restore Zip Archive'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 580
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter: TSplitter
    Left = 0
    Top = 339
    Width = 572
    Height = 6
    Cursor = crVSplit
    Align = alBottom
    AutoSnap = False
    Beveled = True
    MinSize = 36
    ResizeStyle = rsUpdate
  end
  object pnl_Top: TPanel
    Left = 0
    Top = 0
    Width = 572
    Height = 165
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 0
    DesignSize = (
      572
      165)
    object lbl_ArchiveDetails: TLabel
      Left = 152
      Top = 34
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
    object Bevel2: TBevel
      Left = 1
      Top = 125
      Width = 569
      Height = 8
      Anchors = [akLeft, akTop, akRight]
      Shape = bsTopLine
    end
    object lbl_Password: TLabel
      Left = 307
      Top = 102
      Width = 49
      Height = 13
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = '&Password:'
      FocusControl = ed_Password
    end
    object cb_Overwrite: TCheckBox
      Left = 8
      Top = 101
      Width = 257
      Height = 17
      Anchors = [akLeft, akTop, akRight]
      Caption = 'O&verwrite Existing Files and Directories'
      TabOrder = 4
    end
    object Button2: TButton
      Left = 8
      Top = 132
      Width = 233
      Height = 25
      Action = act_RestoreFiles
      TabOrder = 6
    end
    object bt_ZipArchive: TButton
      Left = 8
      Top = 8
      Width = 129
      Height = 23
      Caption = '&Zip Archive'
      TabOrder = 0
      OnClick = bt_ZipArchiveClick
    end
    object ed_Password: TEdit
      Left = 368
      Top = 100
      Width = 195
      Height = 21
      Anchors = [akTop, akRight]
      TabOrder = 5
    end
    object bt_ListFiles: TButton
      Left = 488
      Top = 8
      Width = 75
      Height = 23
      Anchors = [akTop, akRight]
      Caption = '&Open'
      TabOrder = 2
      OnClick = bt_ListFilesClick
    end
    object cmbx_ZipArchiveFilename: TComboBox
      Left = 152
      Top = 9
      Width = 321
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 1
      Text = 'cmbx_ZipArchiveFilename'
      OnChange = cmbx_ZipArchiveFilenameChange
    end
    object gbx_DestDir: TGroupBox
      Left = 8
      Top = 48
      Width = 555
      Height = 44
      Anchors = [akLeft, akTop, akRight]
      Caption = '&Destination Directory'
      TabOrder = 3
      DesignSize = (
        555
        44)
      object ed_DestDir: TEdit
        Left = 8
        Top = 16
        Width = 506
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        Ctl3D = True
        ParentCtl3D = False
        TabOrder = 1
      end
      object bt_DestDir: TButton
        Left = 522
        Top = 16
        Width = 25
        Height = 21
        Anchors = [akTop, akRight]
        Caption = '...'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = bt_DestDirClick
      end
    end
  end
  object gbx_ZipFiles: TGroupBox
    Left = 0
    Top = 165
    Width = 572
    Height = 174
    Align = alClient
    Caption = 'Fi&les'
    TabOrder = 1
    object lv_Files: TListView
      Left = 2
      Top = 15
      Width = 568
      Height = 157
      Align = alClient
      Columns = <
        item
          Caption = 'Name'
          MinWidth = 50
          Width = 100
        end
        item
          Caption = 'Path'
          MinWidth = 50
          Width = 120
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
          Alignment = taRightJustify
          Caption = 'Rate (%)'
        end
        item
          Alignment = taRightJustify
          Caption = 'Compressed'
          Width = 60
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
  object pnl_Bottom: TPanel
    Left = 0
    Top = 345
    Width = 572
    Height = 160
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    object gbx_Log: TGroupBox
      Left = 0
      Top = 0
      Width = 572
      Height = 53
      Align = alClient
      Caption = 'Log'
      Constraints.MinHeight = 36
      TabOrder = 0
      object lb_Log: TListBox
        Left = 2
        Top = 15
        Width = 568
        Height = 36
        Style = lbOwnerDrawFixed
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
        OnDrawItem = lb_LogDrawItem
      end
    end
    object gbx_Progress: TGroupBox
      Left = 0
      Top = 53
      Width = 572
      Height = 71
      Align = alBottom
      Caption = 'Progress'
      TabOrder = 1
      DesignSize = (
        572
        71)
      object lbl_ProgressInfo: TLabel
        Left = 8
        Top = 14
        Width = 75
        Height = 13
        Caption = 'lbl_ProgressInfo'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpVariable
        Font.Style = []
        ParentFont = False
      end
      object bt_Abort: TButton
        Left = 489
        Top = 36
        Width = 75
        Height = 25
        Action = act_Abort
        Anchors = [akTop, akRight]
        TabOrder = 0
      end
      object pnl_ProfileProgress: TPanel
        Left = 8
        Top = 49
        Width = 472
        Height = 18
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 1
        DesignSize = (
          472
          18)
        object Label4: TLabel
          Left = 4
          Top = 2
          Width = 36
          Height = 13
          Caption = 'Archive'
        end
        object lbl_ProfilePerc: TLabel
          Left = 433
          Top = 3
          Width = 31
          Height = 13
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = '100%'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pb_Profile: TProgressBar
          Left = 88
          Top = 1
          Width = 336
          Height = 16
          Anchors = [akLeft, akTop, akRight]
          Min = 0
          Max = 100
          Smooth = True
          TabOrder = 0
        end
      end
      object Panel2: TPanel
        Left = 8
        Top = 31
        Width = 472
        Height = 18
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 2
        DesignSize = (
          472
          18)
        object Label1: TLabel
          Left = 4
          Top = 2
          Width = 16
          Height = 13
          Caption = 'File'
        end
        object lbl_FilePerc: TLabel
          Left = 433
          Top = 3
          Width = 31
          Height = 13
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = '100%'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Pitch = fpVariable
          Font.Style = [fsBold]
          ParentFont = False
        end
        object pb_File: TProgressBar
          Left = 88
          Top = 1
          Width = 336
          Height = 16
          Anchors = [akLeft, akTop, akRight]
          Min = 0
          Max = 100
          Smooth = True
          TabOrder = 0
        end
      end
    end
    object Panel1: TPanel
      Left = 0
      Top = 124
      Width = 572
      Height = 36
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 2
      DesignSize = (
        572
        36)
      object bt_Close: TButton
        Left = 489
        Top = 6
        Width = 75
        Height = 25
        Action = act_Close
        Anchors = [akTop, akRight]
        Cancel = True
        TabOrder = 0
      end
      object Button1: TButton
        Left = 8
        Top = 6
        Width = 129
        Height = 25
        Action = act_SaveLog
        TabOrder = 1
      end
    end
  end
  object ActionList: TActionList
    Left = 320
    Top = 216
    object act_Abort: TAction
      Caption = '&Abort'
      OnExecute = act_AbortExecute
    end
    object act_Close: TAction
      Caption = '&Close'
      ShortCut = 16471
      OnExecute = act_CloseExecute
    end
    object act_SaveLog: TAction
      Caption = '&Save Log'
      ShortCut = 16467
      OnExecute = act_SaveLogExecute
    end
    object act_RestoreFiles: TAction
      Caption = '&Restore Selected Files'
      ShortCut = 16466
      OnExecute = act_RestoreFilesExecute
    end
    object act_SelectAllFiles: TAction
      Caption = 'Select &All'
      ShortCut = 16449
      OnExecute = act_SelectAllFilesExecute
    end
  end
  object sdlg_Log: TSaveDialog
    DefaultExt = 'log'
    Filter = 'Log File (*.log)|*.log|Any File (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Save log file'
    Left = 264
    Top = 216
  end
  object odlg_Backup: TOpenDialog
    DefaultExt = 'zip'
    Filter = 'Zip Files (*.zip)|*.zip|Any File (*.*)|*.*'
    Options = [ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Select an Archive to Restore'
    Left = 392
    Top = 216
  end
  object pop_Files: TPopupMenu
    Left = 104
    Top = 216
    object mi_SelectAll: TMenuItem
      Action = act_SelectAllFiles
    end
  end
  object ZipBuilder: TZipBuilder
    AddStoreSuffixes = [assGIF, assPNG, assZ, assZIP, assZOO, assARC, assLZH, assARJ, assTAZ, assTGZ, assLHA, assRAR, assACE, assCAB, assGZ, assGZIP, assJAR]
    ExtrOptions = [ExtrDirNames, ExtrForceDirs]
    SFXCaption = 'Self-extracting Archive'
    SFXLanguage = 'US'
    SFXOptions = [SFXCheckSize]
    UseDirOnlyEntries = True
    Verbose = True
    OnGetNextDisk = ZipBuilderGetNextDisk
    OnMessage = ZipBuilderMessage
    OnProgressDetails = ZipBuilderProgressDetails
    Left = 48
    Top = 216
  end
end
