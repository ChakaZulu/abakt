object FrameCheckListBox: TFrameCheckListBox
  Left = 0
  Top = 0
  Width = 614
  Height = 208
  TabOrder = 0
  object tbar: TToolBar
    Left = 0
    Top = 0
    Width = 614
    Height = 22
    AutoSize = True
    ButtonWidth = 73
    EdgeBorders = []
    Flat = True
    Images = DM.imgs_Buttons
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = False
    TabOrder = 0
    object tb_Add: TToolButton
      Left = 0
      Top = 0
      AutoSize = True
      Caption = '&Add'
      ImageIndex = 0
    end
    object tb_Remove: TToolButton
      Left = 50
      Top = 0
      AutoSize = True
      Caption = '&Remove'
      ImageIndex = 1
    end
    object tb_Edit: TToolButton
      Left = 121
      Top = 0
      AutoSize = True
      Caption = 'E&dit'
      ImageIndex = 2
    end
    object ToolButton4: TToolButton
      Left = 170
      Top = 0
      Width = 8
      Caption = 'ToolButton4'
      ImageIndex = 3
      Style = tbsSeparator
    end
    object tb_All: TToolButton
      Left = 178
      Top = 0
      AutoSize = True
      Caption = 'A&ll'
      ImageIndex = 4
    end
    object tb_None: TToolButton
      Left = 220
      Top = 0
      AutoSize = True
      Caption = '&None'
      ImageIndex = 5
    end
    object ToolButton7: TToolButton
      Left = 277
      Top = 0
      Width = 8
      Caption = 'ToolButton7'
      ImageIndex = 6
      Style = tbsSeparator
    end
    object tb_Up: TToolButton
      Left = 285
      Top = 0
      Hint = 'Ctrl+Up'
      AutoSize = True
      Caption = 'Up'
      ImageIndex = 6
      ParentShowHint = False
      ShowHint = True
    end
    object tb_Down: TToolButton
      Left = 330
      Top = 0
      Hint = 'Ctrl+Down'
      AutoSize = True
      Caption = 'Down'
      ImageIndex = 7
      ParentShowHint = False
      ShowHint = True
    end
    object tb_Sort: TToolButton
      Left = 389
      Top = 0
      AutoSize = True
      Caption = 'Sort'
      ImageIndex = 8
    end
    object ToolButton12: TToolButton
      Left = 439
      Top = 0
      Width = 8
      Caption = 'ToolButton12'
      ImageIndex = 4
      Style = tbsSeparator
    end
    object tb_Clear: TToolButton
      Left = 447
      Top = 0
      AutoSize = True
      Caption = 'Clear'
      ImageIndex = 3
    end
  end
  object clb: TCheckListBox
    Left = 0
    Top = 22
    Width = 614
    Height = 186
    Align = alClient
    ItemHeight = 13
    TabOrder = 1
  end
end
