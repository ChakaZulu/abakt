object FrameFilterSetList: TFrameFilterSetList
  Left = 0
  Top = 0
  Width = 625
  Height = 241
  TabOrder = 0
  object tbar: TToolBar
    Left = 0
    Top = 0
    Width = 625
    Height = 22
    AutoSize = True
    ButtonWidth = 78
    EdgeBorders = []
    Flat = True
    Images = DM.imgs_Buttons
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = False
    TabOrder = 1
    object tb_Add: TToolButton
      Left = 0
      Top = 0
      Hint = '[Insert]'
      AutoSize = True
      Caption = '&Add'
      ImageIndex = 0
      ParentShowHint = False
      ShowHint = True
      OnClick = tb_AddClick
    end
    object tb_Remove: TToolButton
      Left = 50
      Top = 0
      Hint = '[Delete]'
      AutoSize = True
      Caption = '&Remove'
      ImageIndex = 1
      ParentShowHint = False
      ShowHint = True
      OnClick = tb_RemoveClick
    end
    object tb_Edit: TToolButton
      Left = 121
      Top = 0
      Hint = '[F2]'
      AutoSize = True
      Caption = 'E&dit'
      ImageIndex = 2
      ParentShowHint = False
      ShowHint = True
      OnClick = tb_EditClick
    end
    object tb_Duplicate: TToolButton
      Left = 170
      Top = 0
      AutoSize = True
      Caption = 'Dupli&cate'
      ImageIndex = 9
      OnClick = tb_DuplicateClick
    end
    object ToolButton4: TToolButton
      Left = 246
      Top = 0
      Width = 8
      Caption = 'ToolButton4'
      ImageIndex = 3
      Style = tbsDivider
    end
    object tb_CheckAll: TToolButton
      Left = 254
      Top = 0
      Hint = '[Ctrl+Shift+A]'
      AutoSize = True
      Caption = 'A&ll'
      ImageIndex = 4
      ParentShowHint = False
      ShowHint = True
      OnClick = tb_CheckAllClick
    end
    object tb_CheckNone: TToolButton
      Left = 296
      Top = 0
      Hint = '[Ctrl+Shift+N]'
      AutoSize = True
      Caption = '&None'
      ImageIndex = 5
      ParentShowHint = False
      ShowHint = True
      OnClick = tb_CheckNoneClick
    end
    object ToolButton7: TToolButton
      Left = 353
      Top = 0
      Width = 8
      Caption = 'ToolButton7'
      ImageIndex = 6
      Style = tbsDivider
    end
    object tb_MoveUp: TToolButton
      Left = 361
      Top = 0
      Hint = '[Ctrl+Up]'
      AutoSize = True
      Caption = 'Up'
      ImageIndex = 6
      ParentShowHint = False
      ShowHint = True
      OnClick = tb_MoveUpClick
    end
    object tb_MoveDown: TToolButton
      Left = 406
      Top = 0
      Hint = '[Ctrl+Down]'
      AutoSize = True
      Caption = 'Down'
      ImageIndex = 7
      ParentShowHint = False
      ShowHint = True
      OnClick = tb_MoveDownClick
    end
    object ToolButton12: TToolButton
      Left = 465
      Top = 0
      Width = 8
      Caption = 'ToolButton12'
      ImageIndex = 4
      Style = tbsDivider
    end
    object tb_Clear: TToolButton
      Left = 473
      Top = 0
      AutoSize = True
      Caption = 'Clear'
      ImageIndex = 3
      OnClick = tb_ClearClick
    end
  end
  object lv: TListView
    Left = 0
    Top = 22
    Width = 625
    Height = 219
    Align = alClient
    Checkboxes = True
    Columns = <
      item
        Caption = 'Name'
        Width = 200
      end
      item
        Caption = 'Path Mask'
        Width = 200
      end
      item
        Caption = 'Size'
      end
      item
        Caption = 'Date'
      end
      item
        Caption = 'Age'
      end
      item
        Caption = 'Attributes'
      end>
    HideSelection = False
    Items.Data = {
      21000000010000000000000000000000FFFFFFFF000000000000000004746573
      74}
    ReadOnly = True
    ShowWorkAreas = True
    SmallImages = DM.imgs_Filters
    TabOrder = 0
    ViewStyle = vsReport
    OnDblClick = tb_EditClick
    OnKeyDown = lvKeyDown
    OnMouseDown = lvMouseDown
  end
end
