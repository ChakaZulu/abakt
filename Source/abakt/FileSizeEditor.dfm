object f_FileSizeEditor: Tf_FileSizeEditor
  Left = 491
  Top = 350
  Width = 260
  Height = 200
  Caption = 'Enter File Size'
  Color = clBtnFace
  Constraints.MinHeight = 200
  Constraints.MinWidth = 260
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
  OnShow = FormShow
  DesignSize = (
    252
    170)
  PixelsPerInch = 96
  TextHeight = 13
  object lbl_B: TLabel
    Left = 138
    Top = 12
    Width = 26
    Height = 13
    Caption = '&Bytes'
    FocusControl = ed_B
  end
  object lbl_kB: TLabel
    Left = 138
    Top = 36
    Width = 61
    Height = 13
    Caption = '&kB (rounded)'
    FocusControl = ed_kB
  end
  object lbl_MB: TLabel
    Left = 138
    Top = 60
    Width = 64
    Height = 13
    Caption = '&MB (rounded)'
    FocusControl = ed_MB
  end
  object Label1: TLabel
    Left = 8
    Top = 86
    Width = 237
    Height = 39
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    Caption = 'Use Up/Down keys to modify values.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Pitch = fpVariable
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object ed_B: TEdit
    Left = 8
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 0
    OnChange = ed_BChange
    OnKeyDown = ed_BKeyDown
    OnKeyPress = ed_SizeKeyPress
  end
  object ed_kB: TEdit
    Left = 8
    Top = 32
    Width = 121
    Height = 21
    TabOrder = 1
    OnChange = ed_kBChange
    OnKeyDown = ed_kBKeyDown
    OnKeyPress = ed_SizeKeyPress
  end
  object ed_MB: TEdit
    Left = 8
    Top = 56
    Width = 121
    Height = 21
    TabOrder = 2
    OnChange = ed_MBChange
    OnKeyDown = ed_MBKeyDown
    OnKeyPress = ed_SizeKeyPress
  end
  object Panel1: TPanel
    Left = 0
    Top = 129
    Width = 252
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 3
    DesignSize = (
      252
      41)
    object bt_Ok: TButton
      Left = 78
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object bt_Cancel: TButton
      Left = 166
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
end
