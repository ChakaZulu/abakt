object FrameFilters: TFrameFilters
  Left = 0
  Top = 0
  Width = 647
  Height = 348
  TabOrder = 0
  object pnl_Descr_Filters: TPanel
    Left = 0
    Top = 0
    Width = 647
    Height = 20
    Align = alTop
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 0
    object Label4: TLabel
      Left = 8
      Top = 3
      Width = 322
      Height = 13
      Caption = 
        'Filters will be tested against files in -selected- Source direct' +
        'ories only.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object gbx_FilterSetList: TGroupBox
    Left = 0
    Top = 20
    Width = 647
    Height = 328
    Align = alClient
    Caption = 'F&ilterSet List'
    TabOrder = 1
    inline fr_FilterSetList: TFrameFilterSetList
      Left = 2
      Top = 15
      Width = 643
      Height = 311
      Align = alClient
      TabOrder = 0
      inherited tbar: TToolBar
        Width = 643
        TabOrder = 0
      end
      inherited lv: TListView
        Width = 643
        Height = 259
        TabOrder = 1
      end
    end
  end
end
