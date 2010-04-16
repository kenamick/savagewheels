VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   4530
   ClientLeft      =   60
   ClientTop       =   405
   ClientWidth     =   5340
   LinkTopic       =   "Form1"
   ScaleHeight     =   302
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   356
   StartUpPosition =   2  'CenterScreen
   Begin VB.CheckBox chkNum 
      Caption         =   "sequenced files."
      Height          =   315
      Left            =   3720
      TabIndex        =   14
      Top             =   3360
      Width           =   1455
   End
   Begin VB.PictureBox picTC 
      Height          =   375
      Left            =   4680
      ScaleHeight     =   315
      ScaleWidth      =   435
      TabIndex        =   11
      Top             =   2760
      Width           =   495
   End
   Begin VB.TextBox txtRgb 
      Height          =   285
      Index           =   2
      Left            =   4560
      MaxLength       =   3
      TabIndex        =   7
      Text            =   "0"
      Top             =   2400
      Width           =   615
   End
   Begin VB.TextBox txtRgb 
      Height          =   285
      Index           =   1
      Left            =   4560
      MaxLength       =   3
      TabIndex        =   6
      Text            =   "0"
      Top             =   2160
      Width           =   615
   End
   Begin VB.TextBox txtRgb 
      Height          =   285
      Index           =   0
      Left            =   4560
      MaxLength       =   3
      TabIndex        =   5
      Text            =   "0"
      Top             =   1920
      Width           =   615
   End
   Begin VB.PictureBox picDst 
      AutoRedraw      =   -1  'True
      AutoSize        =   -1  'True
      BorderStyle     =   0  'None
      Height          =   495
      Left            =   120
      ScaleHeight     =   33
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   105
      TabIndex        =   4
      Top             =   2160
      Width           =   1575
   End
   Begin VB.CommandButton Command1 
      Caption         =   "&Close"
      Height          =   495
      Left            =   3720
      TabIndex        =   3
      Top             =   960
      Width           =   1455
   End
   Begin VB.Frame Frame1 
      Caption         =   "Input Picture"
      Height          =   1815
      Left            =   120
      TabIndex        =   1
      Top             =   240
      Width           =   3375
      Begin VB.PictureBox picSrc 
         AutoSize        =   -1  'True
         Height          =   990
         Left            =   120
         ScaleHeight     =   62
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   62
         TabIndex        =   2
         Top             =   360
         Width           =   990
      End
   End
   Begin VB.CommandButton btnProc 
      Caption         =   "&Process"
      Height          =   495
      Left            =   3720
      TabIndex        =   0
      Top             =   360
      Width           =   1455
   End
   Begin VB.Label lblStatus 
      AutoSize        =   -1  'True
      Caption         =   "Ready."
      Height          =   195
      Left            =   120
      TabIndex        =   13
      Top             =   4200
      Width           =   510
   End
   Begin VB.Label Label2 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Transcolor:"
      Height          =   195
      Left            =   4200
      TabIndex        =   12
      Top             =   1560
      Width           =   795
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "B:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Index           =   2
      Left            =   4200
      TabIndex        =   10
      Top             =   2400
      Width           =   195
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "G:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Index           =   1
      Left            =   4200
      TabIndex        =   9
      Top             =   2160
      Width           =   210
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "R:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   195
      Index           =   0
      Left            =   4200
      TabIndex        =   8
      Top             =   1920
      Width           =   210
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Long, ByVal x As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hSrcDC As Long, ByVal xSrc As Long, ByVal ySrc As Long, ByVal dwRop As Long) As Long
Private Declare Function GetPixel Lib "gdi32" (ByVal hdc As Long, ByVal x As Long, ByVal y As Long) As Long

Private ltrans_color As Long
Private in_path      As String
Private out_path     As String
Private bFSQ         As Boolean
Private file_count   As Long


' main()
Private Sub Form_Load()

 ltrans_color = 0
 in_path = App.Path & "\in\"
 out_path = App.Path & "\out\"
 
 picSrc.Left = Frame1.Left + 200
 picSrc.Top = Frame1.Top + 275
 
 Me.Caption = "Pic_Cutter"
 lblStatus.Caption = "Ready."
 
 ' reset params
 file_count = 0
 bFSQ = False
 
End Sub


' process all file in the \In folder
Private Sub btnProc_Click()
    
 Dim file As String
 
 lblStatus.Caption = "Processing..."
 
 ' check for file sequence export
 If (chkNum.Value = vbChecked) Then
  bFSQ = True
 Else
  bFSQ = False
 End If
 
 ' 1st file
 file = Dir(in_path & "*.bmp")
 Do While (file <> "")
 
  ProcessPic (file)
    
  ' next file
  file = Dir
 
 Loop
 
 lblStatus.Caption = "Done..."
 
 file_count = 0
    
End Sub


' procesni kartinkata v srcPic - PBox-a
Private Sub _
ProcessPic(file As String)

   Dim i        As Long
   Dim j        As Long
   Dim width    As Long, height As Long
   Dim w_2      As Long, h_2 As Long
   Dim srcColor As Long
   Dim top_x    As Long, top_y As Long
   Dim bottom_x As Long, bottom_y As Long
   Dim topfound As Boolean, bottomfound As Boolean
   
   
      
   
   ' zaredi kartinkata ot \in
   Set picDst = LoadPicture()
   Set picSrc.Picture = LoadPicture(in_path & file)
   Frame1.width = picSrc.ScaleWidth + 50
   Frame1.height = picSrc.ScaleHeight + 50
   picDst.Top = Frame1.Top + Frame1.height + 15
   DoEvents
   
   
   width = picSrc.ScaleWidth - 1
   height = picSrc.ScaleHeight - 1
   w_2 = width / 2
   h_2 = height / 2
   j = 0
   
   ' Y-axis
   Do While (j < h_2 Or Not (topfound And bottomfound))
    j = j + 1
    
    For i = 0 To width
     
     ' top line
     If (Not topfound) Then
      top_y = j
      srcColor = GetPixel(picSrc.hdc, i, top_y)
      If (srcColor <> ltrans_color) Then topfound = True
     End If
     
     ' bottom line
     If (Not bottomfound) Then
      bottom_y = height - j
      srcColor = GetPixel(picSrc.hdc, i, bottom_y)
      If (srcColor <> ltrans_color) Then bottomfound = True
     End If
     
    Next
   
   Loop
   
   
   topfound = False
   bottomfound = False
   j = 0
   
   ' X-axis
   Do While (j < w_2 Or Not (topfound And bottomfound))
    j = j + 1
    
    For i = 0 To height
     
     ' top line
     If (Not topfound) Then
      top_x = j
      srcColor = GetPixel(picSrc.hdc, top_x, i)
      If (srcColor <> ltrans_color) Then topfound = True
     End If
     
     ' bottom line
     If (Not bottomfound) Then
      bottom_x = width - j
      srcColor = GetPixel(picSrc.hdc, bottom_x, i)
      If (srcColor <> ltrans_color) Then bottomfound = True
     End If
     
    Next
   
   Loop
   
   width = bottom_x - top_x
   height = bottom_y - top_y
   picDst.width = width + 1
   picDst.height = height + 1
   picDst.Refresh
   DoEvents

       
   For j = 0 To height + 1
    For i = 0 To width + 1
     BitBlt picDst.hdc, i, j, 1, 1, picSrc.hdc, top_x + i, top_y + j, vbSrcCopy
    Next
   Next
 
   
  '' save final image
  'picDst.Refresh
  
  DoEvents
  If (bFSQ) Then
   SavePicture picDst.Image, out_path & file_count & ".bmp"
  Else
   SavePicture picDst.Image, out_path & file
  End If
  
  file_count = file_count + 1
  
End Sub


'quit
Private Sub Command1_Click()
 End
End Sub

' change color
Private Sub txtRgb_Change(Index As Integer)
 
 Dim c_val As Long
 
 c_val = RGB(CInt(txtRgb(0).Text), CInt(txtRgb(1).Text), CInt(txtRgb(2).Text))
 picTC.BackColor = c_val
 ltrans_color = c_val

End Sub
