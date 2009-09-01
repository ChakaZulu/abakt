object f_FilterSetEditor: Tf_FilterSetEditor
  Left = 385
  Top = 231
  Width = 470
  Height = 320
  HelpType = htKeyword
  BorderIcons = [biSystemMenu]
  Caption = 'Edit FilterSet'
  Color = clBtnFace
  Constraints.MinHeight = 320
  Constraints.MinWidth = 470
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnl_Properties: TPanel
    Left = 0
    Top = 0
    Width = 462
    Height = 77
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      462
      77)
    object lbl_Name: TLabel
      Left = 8
      Top = 10
      Width = 28
      Height = 13
      Caption = '&Name'
      FocusControl = ed_Name
    end
    object lbl_Action: TLabel
      Left = 8
      Top = 35
      Width = 30
      Height = 13
      Caption = 'A&ction'
      FocusControl = cbex_Action
    end
    object ed_Name: TEdit
      Left = 88
      Top = 8
      Width = 367
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
    object cb_Enabled: TCheckBox
      Left = 6
      Top = 58
      Width = 95
      Height = 17
      Alignment = taLeftJustify
      Caption = '&Enabled'
      TabOrder = 2
    end
    object cbex_Action: TComboBoxEx
      Left = 88
      Top = 32
      Width = 217
      Height = 22
      ItemsEx.CaseSensitive = False
      ItemsEx.SortType = stNone
      ItemsEx = <
        item
        end>
      Style = csExDropDownList
      StyleEx = []
      ItemHeight = 16
      TabOrder = 1
      DropDownCount = 8
    end
  end
  object gbx_Conditions: TGroupBox
    Left = 0
    Top = 77
    Width = 462
    Height = 170
    Align = alClient
    TabOrder = 1
    DesignSize = (
      462
      170)
    object lbl_PathMask: TLabel
      Left = 8
      Top = 42
      Width = 57
      Height = 13
      Caption = '&Path (Mask)'
      FocusControl = ed_PathMask
    end
    object lbl_Size: TLabel
      Left = 8
      Top = 66
      Width = 55
      Height = 13
      Caption = '&Size (Bytes)'
      FocusControl = ed_Size
    end
    object lbl_Age: TLabel
      Left = 8
      Top = 114
      Width = 52
      Height = 13
      Caption = '&Age (Days)'
      FocusControl = ed_Age
    end
    object lbl_Date: TLabel
      Left = 8
      Top = 90
      Width = 86
      Height = 13
      Caption = '&Date (yyyy-mm-dd)'
      FocusControl = ed_Date
    end
    object lbl_Attributes: TLabel
      Left = 8
      Top = 138
      Width = 44
      Height = 13
      Caption = 'A&ttributes'
      FocusControl = ed_Attributes
    end
    object Label2: TLabel
      Left = 8
      Top = 16
      Width = 139
      Height = 13
      Caption = 'File Property Conditions:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Pitch = fpVariable
      Font.Style = [fsBold]
      ParentFont = False
    end
    object ed_PathMask: TEdit
      Left = 136
      Top = 40
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
    object ed_Size: TEdit
      Left = 136
      Top = 64
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
    end
    object ed_Age: TEdit
      Left = 136
      Top = 112
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 3
    end
    object ed_Date: TEdit
      Left = 136
      Top = 88
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 2
    end
    object ed_Attributes: TEdit
      Left = 136
      Top = 136
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 4
    end
  end
  object pnl_Bottom: TPanel
    Left = 0
    Top = 247
    Width = 462
    Height = 40
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    DesignSize = (
      462
      40)
    object bt_Ok: TButton
      Left = 289
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
      OnClick = bt_OkClick
    end
    object bt_Cancel: TButton
      Left = 376
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
      OnClick = bt_CancelClick
    end
    object bt_Reset: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Reset'
      TabOrder = 2
      OnClick = bt_ResetClick
    end
    object bt_Help: TButton
      Left = 96
      Top = 8
      Width = 75
      Height = 25
      Caption = '&Help'
      TabOrder = 3
      OnClick = bt_HelpClick
    end
  end
end
