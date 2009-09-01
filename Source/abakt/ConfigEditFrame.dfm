object FrameConfigEdit: TFrameConfigEdit
  Left = 0
  Top = 0
  Width = 529
  Height = 313
  TabOrder = 0
  object gbx_ProfileProperty: TGroupBox
    Left = 0
    Top = 188
    Width = 529
    Height = 125
    Align = alBottom
    Anchors = [akLeft, akRight]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object lbl_ItemCaption: TLabel
      Left = 7
      Top = 0
      Width = 88
      Height = 13
      Caption = 'lbl_ItemCaption'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Panel5: TPanel
      Left = 2
      Top = 15
      Width = 525
      Height = 86
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 4
      TabOrder = 0
      object pnl_ItemValue: TPanel
        Left = 4
        Top = 4
        Width = 517
        Height = 37
        Align = alTop
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 0
      end
      object sbox_ObjectInfo: TScrollBox
        Left = 4
        Top = 41
        Width = 517
        Height = 41
        Align = alClient
        TabOrder = 1
        object lbl_ObjectInfo: TLabel
          Left = 2
          Top = 0
          Width = 108
          Height = 13
          Caption = 'AUTO_TRANSLATED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
      end
    end
    object Panel2: TPanel
      Left = 2
      Top = 101
      Width = 525
      Height = 22
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        525
        22)
      object lbl_ItemListCaption: TLabel
        Left = 8
        Top = 3
        Width = 99
        Height = 13
        Caption = 'Internal Name=Value'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object ed_InternalNameValue: TEdit
        Left = 160
        Top = 0
        Width = 361
        Height = 19
        Anchors = [akLeft, akTop, akRight]
        AutoSize = False
        BevelOuter = bvNone
        Color = clBtnFace
        ReadOnly = True
        TabOrder = 0
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 529
    Height = 188
    Align = alClient
    BevelOuter = bvNone
    Caption = 'Panel1'
    TabOrder = 0
    object tbar: TToolBar
      Left = 0
      Top = 0
      Width = 529
      Height = 22
      AutoSize = True
      ButtonWidth = 59
      EdgeBorders = []
      Flat = True
      Images = DM.imgs_Buttons
      List = True
      ParentShowHint = False
      ShowCaptions = True
      ShowHint = False
      TabOrder = 0
      object tb_CheckAll: TToolButton
        Left = 0
        Top = 0
        AutoSize = True
        Caption = 'A&ll'
        ImageIndex = 4
        OnClick = tb_CheckAllClick
      end
      object tb_CheckNone: TToolButton
        Left = 42
        Top = 0
        AutoSize = True
        Caption = '&None'
        ImageIndex = 5
        OnClick = tb_CheckNoneClick
      end
    end
    object lv: TListView
      Left = 0
      Top = 22
      Width = 529
      Height = 166
      Align = alClient
      Checkboxes = True
      Columns = <
        item
          Caption = 'Name'
          Width = 200
        end
        item
          Caption = 'Value'
          Width = 240
        end
        item
          Caption = 'Type'
          Width = 80
        end>
      ColumnClick = False
      HideSelection = False
      ReadOnly = True
      StateImages = DM.imgs_CheckBoxes
      TabOrder = 1
      ViewStyle = vsReport
      OnCustomDrawItem = lvCustomDrawItem
      OnKeyPress = lvKeyPress
      OnMouseDown = lvMouseDown
      OnSelectItem = lvSelectItem
    end
  end
end
