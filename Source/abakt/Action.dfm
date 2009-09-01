object f_Action: Tf_Action
  Left = 406
  Top = 245
  Width = 500
  Height = 400
  Caption = 'Action'
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 333
    Width = 492
    Height = 34
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      492
      34)
    object bt_Close: TButton
      Left = 407
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = '&Close'
      TabOrder = 0
      OnClick = bt_CloseClick
    end
    object bt_SaveLog: TButton
      Left = 8
      Top = 6
      Width = 121
      Height = 25
      Caption = '&Save Log'
      TabOrder = 1
      OnClick = bt_SaveLogClick
    end
  end
  object gbx_Log: TGroupBox
    Left = 0
    Top = 61
    Width = 492
    Height = 189
    Align = alClient
    Caption = '&Log'
    TabOrder = 1
    object lb_Log: TListBox
      Left = 2
      Top = 15
      Width = 488
      Height = 172
      Style = lbOwnerDrawFixed
      AutoComplete = False
      Align = alClient
      ItemHeight = 13
      ScrollWidth = 2048
      TabOrder = 0
      OnDrawItem = lb_LogDrawItem
    end
  end
  object gbx_Progress: TGroupBox
    Left = 0
    Top = 250
    Width = 492
    Height = 83
    Align = alBottom
    Caption = 'Progress'
    TabOrder = 2
    DesignSize = (
      492
      83)
    object lbl_ProgressInfo: TLabel
      Left = 8
      Top = 14
      Width = 75
      Height = 13
      Caption = 'lbl_ProgressInfo'
    end
    object bt_Abort: TButton
      Left = 407
      Top = 42
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '&Abort'
      TabOrder = 0
      OnClick = bt_AbortClick
    end
    object pnl_GroupProgress: TPanel
      Left = 8
      Top = 63
      Width = 390
      Height = 16
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
      DesignSize = (
        390
        16)
      object Label5: TLabel
        Left = 4
        Top = 1
        Width = 29
        Height = 13
        Caption = 'Group'
      end
      object lbl_GroupPerc: TLabel
        Left = 351
        Top = 1
        Width = 31
        Height = 13
        Alignment = taRightJustify
        Anchors = [akTop, akRight]
        Caption = '100%'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object pb_Group: TProgressBar
        Left = 64
        Top = 1
        Width = 278
        Height = 14
        Anchors = [akLeft, akTop, akRight]
        Min = 0
        Max = 100
        Smooth = True
        TabOrder = 0
      end
    end
    object pnl_ProfileProgress: TPanel
      Left = 8
      Top = 47
      Width = 390
      Height = 16
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 2
      DesignSize = (
        390
        16)
      object Label4: TLabel
        Left = 4
        Top = 1
        Width = 29
        Height = 13
        Caption = 'Profile'
      end
      object lbl_ProfilePerc: TLabel
        Left = 351
        Top = 1
        Width = 31
        Height = 13
        Alignment = taRightJustify
        Anchors = [akTop, akRight]
        Caption = '100%'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object pb_Profile: TProgressBar
        Left = 64
        Top = 1
        Width = 278
        Height = 14
        Anchors = [akLeft, akTop, akRight]
        Min = 0
        Max = 100
        Smooth = True
        TabOrder = 0
      end
    end
    object Panel3: TPanel
      Left = 8
      Top = 31
      Width = 390
      Height = 16
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 3
      DesignSize = (
        390
        16)
      object lbl_File: TLabel
        Left = 4
        Top = 1
        Width = 16
        Height = 13
        Caption = 'File'
      end
      object lbl_FilePerc: TLabel
        Left = 351
        Top = 1
        Width = 31
        Height = 13
        Alignment = taRightJustify
        Anchors = [akTop, akRight]
        Caption = '100%'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object pb_File: TProgressBar
        Left = 64
        Top = 1
        Width = 278
        Height = 14
        Anchors = [akLeft, akTop, akRight]
        Min = 0
        Max = 100
        Smooth = True
        TabOrder = 0
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 492
    Height = 61
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 3
    DesignSize = (
      492
      61)
    object Label1: TLabel
      Left = 8
      Top = 8
      Width = 29
      Height = 13
      Caption = 'Profile'
    end
    object Label2: TLabel
      Left = 8
      Top = 26
      Width = 53
      Height = 13
      Caption = 'Destination'
    end
    object Label3: TLabel
      Left = 8
      Top = 44
      Width = 29
      Height = 13
      Caption = 'Group'
    end
    object ed_ProfilePath: TEdit
      Left = 80
      Top = 6
      Width = 345
      Height = 17
      Anchors = [akLeft, akTop, akRight]
      BevelKind = bkFlat
      BorderStyle = bsNone
      Color = clSilver
      Ctl3D = True
      ParentCtl3D = False
      ReadOnly = True
      TabOrder = 0
      Text = 'ed_ProfilePath'
    end
    object ed_Destination: TEdit
      Left = 80
      Top = 24
      Width = 345
      Height = 17
      Anchors = [akLeft, akTop, akRight]
      BevelKind = bkFlat
      BorderStyle = bsNone
      Color = clSilver
      Ctl3D = True
      ParentCtl3D = False
      ReadOnly = True
      TabOrder = 1
      Text = 'ed_Destination'
    end
    object ed_Group: TEdit
      Left = 80
      Top = 42
      Width = 288
      Height = 17
      Anchors = [akLeft, akTop, akRight]
      BevelKind = bkFlat
      BorderStyle = bsNone
      Color = clSilver
      Ctl3D = True
      ParentCtl3D = False
      ReadOnly = True
      TabOrder = 2
      Text = 'ed_Group'
    end
    object pnl_ElapsedTime: TPanel
      Left = 430
      Top = 6
      Width = 56
      Height = 18
      Anchors = [akTop, akRight]
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = '9:59:59'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
    end
    object pnl_GroupElapsedTime: TPanel
      Left = 430
      Top = 42
      Width = 56
      Height = 18
      Anchors = [akTop, akRight]
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = '9:59:59'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
    end
    object pnl_GroupCount: TPanel
      Left = 372
      Top = 42
      Width = 54
      Height = 18
      Anchors = [akTop, akRight]
      BevelInner = bvRaised
      BevelOuter = bvLowered
      Caption = '100/100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 5
    end
  end
  object sdlg_Log: TSaveDialog
    DefaultExt = 'log'
    Filter = 'Log File (*.log)|*.log|Any File (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Save log file'
    Left = 48
    Top = 144
  end
  object tmr_Action: TTimer
    Enabled = False
    OnTimer = tmr_ActionTimer
    Left = 112
    Top = 144
  end
  object ZipBuilder: TZipBuilder
    AddCompLevel = 6
    AddOptions = [AddDirNames, AddHiddenFiles, AddSeparateDirs, AddSafe]
    AddStoreSuffixes = [assEXT]
    ExtrOptions = [ExtrDirNames]
    SFXCaption = 'Self-extracting Archive'
    SFXLanguage = 'US'
    SFXOptions = [SFXCheckSize]
    UseDirOnlyEntries = True
    Verbose = True
    OnGetNextDisk = ZipBuilderGetNextDisk
    OnMessage = ZipBuilderMessage
    OnProgressDetails = ZipBuilderProgressDetails
    OnSetAddName = ZipBuilderSetAddName
    OnStatusDisk = ZipBuilderStatusDisk
    OnZipDialog = ZipBuilderZipDialog
    Left = 48
    Top = 88
  end
end
