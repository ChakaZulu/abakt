object f_About: Tf_About
  Left = 481
  Top = 204
  Width = 360
  Height = 400
  BorderIcons = [biSystemMenu]
  Caption = 'About'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 360
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  ShowHint = True
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 252
    Width = 352
    Height = 38
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 0
    DesignSize = (
      352
      38)
    object Label11: TLabel
      Left = 16
      Top = 4
      Width = 321
      Height = 13
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Caption = '&Support Abakt by making a small donation:'
      FocusControl = pnl_MakeDonation
    end
    object pnl_MakeDonation: TPanel
      Left = 16
      Top = 19
      Width = 321
      Height = 13
      Cursor = crHandPoint
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      Caption = 'Visit the website to read more.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = pnl_MakeDonationClick
      OnEnter = pnl_MakeDonationClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 328
    Width = 352
    Height = 39
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      352
      39)
    object bt_Close: TButton
      Left = 138
      Top = 9
      Width = 75
      Height = 25
      Anchors = []
      Cancel = True
      Caption = '&Close'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 290
    Width = 352
    Height = 38
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 2
    DesignSize = (
      352
      38)
    object lbl_ZipBuilder: TLabel
      Left = 16
      Top = 4
      Width = 321
      Height = 15
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Caption = 'Abakt uses ZipBuilder %s.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object lbl_ZipBuilderLink: TPanel
      Left = 16
      Top = 19
      Width = 321
      Height = 13
      Cursor = crHandPoint
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      Caption = 'See http://www.drehoeksw.net/'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = lbl_ZipBuilderLinkClick
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 227
    Width = 352
    Height = 25
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 3
    object Label10: TLabel
      Left = 8
      Top = 6
      Width = 22
      Height = 13
      Caption = 'User'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
    object lbl_User: TLabel
      Left = 96
      Top = 6
      Width = 22
      Height = 13
      Caption = 'User'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
    end
  end
  object Panel5: TPanel
    Left = 0
    Top = 185
    Width = 352
    Height = 42
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 4
    DesignSize = (
      352
      42)
    object Label9: TLabel
      Left = 8
      Top = 6
      Width = 48
      Height = 13
      Caption = 'Language'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
    object Label12: TLabel
      Left = 8
      Top = 22
      Width = 47
      Height = 13
      Caption = 'Translator'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
    object lbl_Language: TLabel
      Left = 96
      Top = 6
      Width = 48
      Height = 13
      Caption = 'Language'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      Transparent = True
    end
    object ed_Translator: TEdit
      Left = 96
      Top = 23
      Width = 241
      Height = 13
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      BevelInner = bvNone
      BevelOuter = bvNone
      BorderStyle = bsNone
      Color = clBtnFace
      Ctl3D = False
      ParentCtl3D = False
      ReadOnly = True
      TabOrder = 0
      Text = 'Translator'
    end
  end
  object Panel6: TPanel
    Left = 0
    Top = 129
    Width = 352
    Height = 56
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 5
    object lbl_Released: TLabel
      Left = 96
      Top = 22
      Width = 23
      Height = 13
      Caption = 'Date'
      Transparent = True
    end
    object lbl_Version: TLabel
      Left = 96
      Top = 6
      Width = 35
      Height = 13
      Caption = 'Version'
      Transparent = True
    end
    object Label5: TLabel
      Left = 8
      Top = 6
      Width = 35
      Height = 13
      Caption = 'Version'
      Transparent = True
    end
    object Label6: TLabel
      Left = 8
      Top = 22
      Width = 23
      Height = 13
      Caption = 'Date'
      Transparent = True
    end
    object Label8: TLabel
      Left = 8
      Top = 38
      Width = 48
      Height = 13
      Caption = 'Using ecc'
      Transparent = True
    end
    object lbl_EccVersion: TLabel
      Left = 96
      Top = 38
      Width = 48
      Height = 13
      Caption = 'Using ecc'
      Transparent = True
    end
  end
  object Panel7: TPanel
    Left = 0
    Top = 57
    Width = 352
    Height = 72
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 6
    DesignSize = (
      352
      72)
    object lbl_Author: TLabel
      Left = 8
      Top = 6
      Width = 31
      Height = 13
      Caption = 'Author'
      Transparent = True
    end
    object Label2: TLabel
      Left = 8
      Top = 53
      Width = 37
      Height = 13
      Caption = '&License'
      FocusControl = pnl_License
    end
    object Label3: TLabel
      Left = 8
      Top = 38
      Width = 28
      Height = 13
      Caption = '&E-mail'
      FocusControl = pnl_Email
      Transparent = True
    end
    object Label4: TLabel
      Left = 8
      Top = 22
      Width = 36
      Height = 13
      Caption = '&Internet'
      FocusControl = pnl_Internet
      Transparent = True
    end
    object Label7: TLabel
      Left = 96
      Top = 6
      Width = 60
      Height = 13
      Caption = 'Erik Dienske'
      Transparent = True
    end
    object pnl_Email: TPanel
      Left = 96
      Top = 38
      Width = 240
      Height = 13
      Cursor = crHandPoint
      Alignment = taLeftJustify
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      Caption = 'abakt@xs4all.nl'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = pnl_EmailClick
      OnEnter = pnl_EmailClick
    end
    object pnl_Internet: TPanel
      Left = 96
      Top = 22
      Width = 240
      Height = 13
      Cursor = crHandPoint
      Alignment = taLeftJustify
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      Caption = 'http://www.xs4all.nl/~edienske/abakt/'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = pnl_InternetClick
      OnEnter = pnl_InternetClick
    end
    object pnl_License: TPanel
      Left = 96
      Top = 53
      Width = 240
      Height = 13
      Cursor = crHandPoint
      Alignment = taLeftJustify
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      Caption = 'GNU General Public License'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = pnl_LicenseClick
      OnEnter = pnl_LicenseClick
    end
  end
  object Panel8: TPanel
    Left = 0
    Top = 0
    Width = 352
    Height = 57
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 7
    object lbl_AppName: TLabel
      Left = 96
      Top = 12
      Width = 130
      Height = 13
      Caption = 'Abakt - A Backup Tool'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = [fsBold]
      ParentFont = False
      Transparent = True
    end
    object Label1: TLabel
      Left = 96
      Top = 30
      Width = 183
      Height = 13
      Caption = 'Copyright (C) 2003-2006 - Erik Dienske'
    end
    object img_Logo: TImage
      Left = 32
      Top = 12
      Width = 32
      Height = 32
    end
  end
end
