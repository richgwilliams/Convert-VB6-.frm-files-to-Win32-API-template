Begin VB.Form frmCommand 
   Appearance      =   0  'Flat
   BackColor       =   &H00000090&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Spectrel Toolbox"
   ClientHeight    =   11370
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   19110
   FillColor       =   &H00000090&
   ForeColor       =   &H00000090&
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   11370
   ScaleMode       =   0  'User
   ScaleWidth      =   19095.01
   Begin VB.CommandButton cmdLogs 
      BackColor       =   &H0000C0C0&
      Caption         =   "Log or Spectrum"
      Height          =   700
      Left            =   7800
      Style           =   1  'Graphical
      TabIndex        =   30
      Top             =   9120
      Width           =   1000
   End
   Begin VB.TextBox txtError 
      Appearance      =   0  'Flat
      BackColor       =   &H00000090&
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   825
      Left            =   14640
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   1200
      Width           =   4335
   End
   Begin VB.TextBox memConvert 
      Alignment       =   2  'Center
      BackColor       =   &H00000090&
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "Verdana"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   340
      Left            =   10200
      Locked          =   -1  'True
      TabIndex        =   28
      TabStop         =   0   'False
      Top             =   9960
      Width           =   1000
   End
 End
