/**
 * @file  ToolWindowEdit.h
 * @brief Preferences dialog.
 *
 */
/*
 * Original Author: Ruopeng Wang
 * CVS Revision Info:
 *    $Author: rpwang $
 *    $Date: 2009/07/07 22:05:04 $
 *    $Revision: 1.12 $
 *
 * Copyright (C) 2008-2009,
 * The General Hospital Corporation (Boston, MA).
 * All rights reserved.
 *
 * Distribution, usage and copying of this software is covered under the
 * terms found in the License Agreement file named 'COPYING' found in the
 * FreeSurfer source code root directory, and duplicated here:
 * https://surfer.nmr.mgh.harvard.edu/fswiki/FreeSurferOpenSourceLicense
 *
 * General inquiries: freesurfer@nmr.mgh.harvard.edu
 * Bug reports: analysis-bugs@nmr.mgh.harvard.edu
 *
 */



#include "ToolWindowEdit.h"
#include <wx/wx.h>
#include <wx/config.h>
#include <wx/xrc/xmlres.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/spinctrl.h>
#include "MainWindow.h"
#include "RenderView2D.h"
#include "RenderView3D.h"
#include "BrushProperty.h"
#include "Interactor2DROIEdit.h"
#include "Interactor2DVoxelEdit.h"
#include "LayerCollection.h"
#include "LayerMRI.h"
#include "LayerPropertiesMRI.h"
#include "LayerROI.h"
#include "LayerDTI.h"

BEGIN_EVENT_TABLE( ToolWindowEdit, wxFrame )
// EVT_BUTTON   ( wxID_OK,          ToolWindowEdit::OnOK )
// EVT_BUTTON   ( XRCID( wxT( "ID_BUTTON_VECTOR_FILE" ) ), ToolWindowEdit::OnButtonVector )
// EVT_BUTTON   ( XRCID( wxT( "ID_BUTTON_FA_FILE" ) ),  ToolWindowEdit::OnButtonFA )
// EVT_COMBOBOX  ( XRCID( wxT( "ID_COMBO_FA_FILE" ) ),   ToolWindowEdit::OnComboFASelectionChanged )
EVT_MENU      ( XRCID( "ID_ACTION_VOXEL_FREEHAND" ),  ToolWindowEdit::OnActionVoxelFreehand )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_VOXEL_FREEHAND" ),  ToolWindowEdit::OnActionVoxelFreehandUpdateUI )
EVT_MENU      ( XRCID( "ID_ACTION_VOXEL_FILL" ),      ToolWindowEdit::OnActionVoxelFill )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_VOXEL_FILL" ),      ToolWindowEdit::OnActionVoxelFillUpdateUI )
EVT_MENU      ( XRCID( "ID_ACTION_VOXEL_POLYLINE" ),  ToolWindowEdit::OnActionVoxelPolyline )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_VOXEL_POLYLINE" ),  ToolWindowEdit::OnActionVoxelPolylineUpdateUI )
EVT_MENU      ( XRCID( "ID_ACTION_VOXEL_LIVEWIRE" ),  ToolWindowEdit::OnActionVoxelLivewire )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_VOXEL_LIVEWIRE" ),  ToolWindowEdit::OnActionVoxelLivewireUpdateUI )
EVT_MENU      ( XRCID( "ID_ACTION_VOXEL_EYEDROP" ),   ToolWindowEdit::OnActionVoxelColorPicker )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_VOXEL_EYEDROP" ),   ToolWindowEdit::OnActionVoxelColorPickerUpdateUI )

EVT_MENU      ( XRCID( "ID_ACTION_VOXEL_LIVEWIRE" ),  ToolWindowEdit::OnActionVoxelLivewire )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_VOXEL_LIVEWIRE" ),  ToolWindowEdit::OnActionVoxelLivewireUpdateUI )

EVT_MENU      ( XRCID( "ID_ACTION_ROI_FREEHAND" ),    ToolWindowEdit::OnActionROIFreehand )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_ROI_FREEHAND" ),    ToolWindowEdit::OnActionROIFreehandUpdateUI )
EVT_MENU      ( XRCID( "ID_ACTION_ROI_FILL" ),        ToolWindowEdit::OnActionROIFill )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_ROI_FILL" ),        ToolWindowEdit::OnActionROIFillUpdateUI )
EVT_MENU      ( XRCID( "ID_ACTION_ROI_POLYLINE" ),    ToolWindowEdit::OnActionROIPolyline )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_ROI_POLYLINE" ),    ToolWindowEdit::OnActionROIPolylineUpdateUI )
EVT_MENU      ( XRCID( "ID_ACTION_ROI_LIVEWIRE" ),    ToolWindowEdit::OnActionROILivewire )
EVT_UPDATE_UI ( XRCID( "ID_ACTION_ROI_LIVEWIRE" ),    ToolWindowEdit::OnActionROILivewireUpdateUI )

EVT_SPINCTRL  ( XRCID( "ID_SPIN_BRUSH_SIZE" ),        ToolWindowEdit::OnSpinBrushSize )
EVT_SPINCTRL  ( XRCID( "ID_SPIN_BRUSH_TOLERANCE" ),   ToolWindowEdit::OnSpinBrushTolerance )
EVT_CHOICE    ( XRCID( "ID_CHOICE_REFERENCE" ),       ToolWindowEdit::OnChoiceBrushTemplate )
EVT_CHECKBOX  ( XRCID( "ID_CHECK_DRAW_CONNECTED" ),   ToolWindowEdit::OnCheckDrawConnectedOnly )
EVT_CHECKBOX  ( XRCID( "ID_CHECK_DRAW_RANGE" ),       ToolWindowEdit::OnCheckDrawRange )
EVT_CHECKBOX  ( XRCID( "ID_CHECK_EXCLUDE_RANGE" ),    ToolWindowEdit::OnCheckExcludeRange )
EVT_TEXT      ( XRCID( "ID_EDIT_DRAW_RANGE_LOW" ),    ToolWindowEdit::OnEditDrawRangeLow )
EVT_TEXT      ( XRCID( "ID_EDIT_DRAW_RANGE_HIGH" ),   ToolWindowEdit::OnEditDrawRangeHigh )
EVT_TEXT      ( XRCID( "ID_EDIT_EXCLUDE_RANGE_LOW" ), ToolWindowEdit::OnEditExcludeRangeLow )
EVT_TEXT      ( XRCID( "ID_EDIT_EXCLUDE_RANGE_HIGH" ),  ToolWindowEdit::OnEditExcludeRangeHigh )

EVT_SHOW      ( ToolWindowEdit::OnShow )

END_EVENT_TABLE()


ToolWindowEdit::ToolWindowEdit( wxWindow* parent ) :
    m_bToUpdateTools( false )
{
  wxXmlResource::Get()->LoadFrame( this, parent, wxT("ID_TOOLWINDOW_EDIT") );
  m_toolbarVoxelEdit    = XRCCTRL( *this, "ID_TOOLBAR_VOXEL_EDIT",    wxToolBar );
  m_toolbarROIEdit      = XRCCTRL( *this, "ID_TOOLBAR_ROI_EDIT",      wxToolBar );
  m_spinBrushSize       = XRCCTRL( *this, "ID_SPIN_BRUSH_SIZE",       wxSpinCtrl );
  m_spinBrushTolerance  = XRCCTRL( *this, "ID_SPIN_BRUSH_TOLERANCE",  wxSpinCtrl );
  m_choiceTemplate      = XRCCTRL( *this, "ID_CHOICE_REFERENCE",      wxChoice );
  m_checkDrawRange      = XRCCTRL( *this, "ID_CHECK_DRAW_RANGE",      wxCheckBox );
  m_checkExcludeRange   = XRCCTRL( *this, "ID_CHECK_EXCLUDE_RANGE",   wxCheckBox );
  m_editDrawRangeLow    = XRCCTRL( *this, "ID_EDIT_DRAW_RANGE_LOW",   wxTextCtrl );
  m_editDrawRangeHigh   = XRCCTRL( *this, "ID_EDIT_DRAW_RANGE_HIGH",  wxTextCtrl );
  m_editExcludeRangeLow     = XRCCTRL( *this, "ID_EDIT_EXCLUDE_RANGE_LOW",  wxTextCtrl );
  m_editExcludeRangeHigh    = XRCCTRL( *this, "ID_EDIT_EXCLUDE_RANGE_HIGH", wxTextCtrl );
  m_checkDrawConnectedOnly  = XRCCTRL( *this, "ID_CHECK_DRAW_CONNECTED",    wxCheckBox );
}

ToolWindowEdit::~ToolWindowEdit()
{}

void ToolWindowEdit::OnShow( wxShowEvent& event )
{
  if ( event.GetShow() )
  {
    wxConfigBase* config = wxConfigBase::Get();
    if ( config )
    {
      int x = config->Read( _T("/ToolWindowEdit/PosX"), 50L );
      int y = config->Read( _T("/ToolWindowEdit/PosY"), 50L );
      Move( x, y );
    }
  }
  else
  {
    wxConfigBase* config = wxConfigBase::Get();
    if ( config )
    {
      int x, y;
      GetPosition( &x, &y );
      config->Write( _T("/ToolWindowEdit/PosX"), (long) x );
      config->Write( _T("/ToolWindowEdit/PosY"), (long) y );
    }
  }
}

void ToolWindowEdit::ResetPosition()
{
  if ( IsShown() )
  {
    wxConfigBase* config = wxConfigBase::Get();
    if ( config )
    {
      int x = config->Read( _T("/ToolWindowEdit/PosX"), 50L );
      int y = config->Read( _T("/ToolWindowEdit/PosY"), 50L );
      Move( x, y );
    }
  }
}

void ToolWindowEdit::UpdateTools()
{
  m_bToUpdateTools = true;
}

void ToolWindowEdit::DoUpdateTools()
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );

  bool bVoxelEditVisible = m_toolbarVoxelEdit->IsShown();
  bool bROIEditVisible = m_toolbarROIEdit->IsShown();
  if ( bVoxelEditVisible != (view->GetInteractionMode() == RenderView2D::IM_VoxelEdit) ||
       bROIEditVisible != (view->GetInteractionMode() == RenderView2D::IM_ROIEdit) )
  {
    m_toolbarVoxelEdit ->Show( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit );
    m_toolbarROIEdit ->Show( view->GetInteractionMode() == RenderView2D::IM_ROIEdit );

    XRCCTRL( *this, "ID_PANEL_HOLDER", wxPanel )->Layout();
  }

// XRCCTRL( *m_toolbarBrush, "ID_STATIC_BRUSH_SIZE", wxStaticText )->Enable( m_viewAxial->GetAction() != Interactor2DROIEdit::EM_Fill );
  m_spinBrushSize->Enable( view->GetAction() != Interactor2DROIEdit::EM_Fill );
  m_spinBrushTolerance->Enable( view->GetAction() == Interactor2DROIEdit::EM_Fill );
// choiceTemplate->Enable( checkTemplate->IsChecked() && m_viewAxial->GetAction() == Interactor2DROIEdit::EM_Fill );
// XRCCTRL( *m_toolbarBrush, "ID_STATIC_BRUSH_TOLERANCE", wxStaticText )->Enable( checkTemplate->IsChecked() ); //&& m_viewAxial->GetAction() == Interactor2DROIEdit::EM_Fill );
// XRCCTRL( *m_toolbarBrush, "ID_SPIN_BRUSH_TOLERANCE", wxSpinCtrl )->Enable( checkTemplate->IsChecked() );//&& m_viewAxial->GetAction() == Interactor2DROIEdit::EM_Fill );

  BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
  LayerVolumeBase* layer = bp->GetReferenceLayer();
// if ( m_choiceTemplate->GetSelection() != wxNOT_FOUND )
//  layer = ( LayerEditable* )(void*)m_choiceTemplate->GetClientData( m_choiceTemplate->GetSelection() );

  m_choiceTemplate->Clear();
  m_choiceTemplate->Append( _("None"), (void*)NULL );
  LayerCollection* lc = MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" );
  int nSel = 0;
  for ( int i = 0; i < lc->GetNumberOfLayers(); i++ )
  {
    LayerMRI* mri = (LayerMRI*)lc->GetLayer( i );
    if ( layer == mri )
    {
      nSel = i+1;
    }

    m_choiceTemplate->Append( wxString::FromAscii( mri->GetName() ), (void*)mri );
  }
// if ( !lc->IsEmpty() )
  m_choiceTemplate->SetSelection( nSel );

  m_spinBrushSize->SetValue( bp->GetBrushSize() );
  m_spinBrushTolerance->SetValue( bp->GetBrushTolerance( ) );

  m_checkDrawConnectedOnly->SetValue( bp->GetDrawConnectedOnly() );
  m_checkDrawRange  ->SetValue( bp->GetDrawRangeEnabled() );
  m_checkExcludeRange  ->SetValue( bp->GetExcludeRangeEnabled() );

  m_editDrawRangeLow  ->Enable( bp->GetDrawRangeEnabled() );
  m_editDrawRangeHigh  ->Enable( bp->GetDrawRangeEnabled() );
  m_editExcludeRangeLow ->Enable( bp->GetExcludeRangeEnabled() );
  m_editExcludeRangeHigh ->Enable( bp->GetExcludeRangeEnabled() );

  double* range = bp->GetDrawRange();
  UpdateTextValue( m_editDrawRangeLow, range[0] );
  UpdateTextValue( m_editDrawRangeHigh, range[1] );
  range = bp->GetExcludeRange();
  UpdateTextValue( m_editExcludeRangeLow, range[0] );
  UpdateTextValue( m_editExcludeRangeHigh, range[1] );

  m_bToUpdateTools = false;
}

void ToolWindowEdit::UpdateTextValue( wxTextCtrl* ctrl, double dvalue )
{
  wxString value_strg = ( wxString() << dvalue );
  if ( value_strg != ctrl->GetValue() && (value_strg + _(".")) != ctrl->GetValue() )
    ctrl->ChangeValue( value_strg );
}

void ToolWindowEdit::OnInternalIdle()
{
  wxFrame::OnInternalIdle();

  if ( m_bToUpdateTools )
    DoUpdateTools();
}

void ToolWindowEdit::OnActionVoxelFreehand( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DVoxelEdit::EM_Freehand );
  UpdateTools();
}

void ToolWindowEdit::OnActionVoxelFreehandUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
               && view->GetAction() == Interactor2DVoxelEdit::EM_Freehand );
  event.Enable( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
                && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}


void ToolWindowEdit::OnActionVoxelPolyline( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DVoxelEdit::EM_Polyline );
  UpdateTools();
}

void ToolWindowEdit::OnActionVoxelPolylineUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
               && view->GetAction() == Interactor2DVoxelEdit::EM_Polyline );

  event.Enable( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
                && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}


void ToolWindowEdit::OnActionVoxelLivewire( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DVoxelEdit::EM_Livewire );
  UpdateTools();
}

void ToolWindowEdit::OnActionVoxelLivewireUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
               && view->GetAction() == Interactor2DVoxelEdit::EM_Livewire );

  event.Enable( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
                && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}


void ToolWindowEdit::OnActionVoxelFill( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DVoxelEdit::EM_Fill );
  UpdateTools();
}

void ToolWindowEdit::OnActionVoxelFillUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
               && view->GetAction() == Interactor2DVoxelEdit::EM_Fill );

  event.Enable( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
                && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}

void ToolWindowEdit::OnActionVoxelColorPicker( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DVoxelEdit::EM_ColorPicker );
  UpdateTools();
}

void ToolWindowEdit::OnActionVoxelColorPickerUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
      && view->GetAction() == Interactor2DVoxelEdit::EM_ColorPicker );

  event.Enable( view->GetInteractionMode() == RenderView2D::IM_VoxelEdit
      && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}


void ToolWindowEdit::OnActionROIFreehand( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DROIEdit::EM_Freehand );
  UpdateTools();
}

void ToolWindowEdit::OnActionROIFreehandUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_ROIEdit
               && view->GetAction() == Interactor2DROIEdit::EM_Freehand );
  event.Enable( view->GetInteractionMode() == RenderView2D::IM_ROIEdit
                && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}


void ToolWindowEdit::OnActionROIPolyline( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DROIEdit::EM_Polyline );
  UpdateTools();
}

void ToolWindowEdit::OnActionROIPolylineUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_ROIEdit
               && view->GetAction() == Interactor2DROIEdit::EM_Polyline );

  event.Enable( view->GetInteractionMode() == RenderView2D::IM_ROIEdit
                && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}


void ToolWindowEdit::OnActionROILivewire( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DROIEdit::EM_Livewire );
  UpdateTools();
}

void ToolWindowEdit::OnActionROILivewireUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_ROIEdit
               && view->GetAction() == Interactor2DROIEdit::EM_Livewire );

  event.Enable( view->GetInteractionMode() == RenderView2D::IM_ROIEdit
                && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}


void ToolWindowEdit::OnActionROIFill( wxCommandEvent& event )
{
  MainWindow::GetMainWindowPointer()->SetAction( Interactor2DROIEdit::EM_Fill );
  UpdateTools();
}

void ToolWindowEdit::OnActionROIFillUpdateUI( wxUpdateUIEvent& event)
{
  RenderView2D* view = ( RenderView2D* )MainWindow::GetMainWindowPointer()->GetRenderView( 0 );
  event.Check( view->GetInteractionMode() == RenderView2D::IM_ROIEdit
               && view->GetAction() == Interactor2DROIEdit::EM_Fill );

  event.Enable( view->GetInteractionMode() == RenderView2D::IM_ROIEdit
                && !MainWindow::GetMainWindowPointer()->GetLayerCollection( "MRI" )->IsEmpty() );
}

void ToolWindowEdit::OnSpinBrushSize( wxSpinEvent& event )
{
  BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
  bp->SetBrushSize( event.GetInt() );
  UpdateTools();
}

void ToolWindowEdit::OnSpinBrushTolerance( wxSpinEvent& event )
{
  BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
  bp->SetBrushTolerance( event.GetInt() );
  UpdateTools();
}

void ToolWindowEdit::OnChoiceBrushTemplate( wxCommandEvent& event )
{
  LayerVolumeBase* layer = (LayerVolumeBase*)(void*)m_choiceTemplate->GetClientData( event.GetSelection() );
  BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
  bp->SetReferenceLayer( layer );
  UpdateTools();
}

void ToolWindowEdit::OnCheckDrawConnectedOnly( wxCommandEvent& event )
{
  BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
  bp->SetDrawConnectedOnly( event.IsChecked() );

  UpdateTools();
}

void ToolWindowEdit::OnCheckDrawRange( wxCommandEvent& event )
{
  BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
  bp->SetDrawRangeEnabled( event.IsChecked() );

  UpdateTools();
}

void ToolWindowEdit::OnCheckExcludeRange( wxCommandEvent& event )
{
  BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
  bp->SetExcludeRangeEnabled( event.IsChecked() );
  UpdateTools();
}

void ToolWindowEdit::OnEditDrawRangeLow( wxCommandEvent& event )
{
  double value;
  if ( m_editDrawRangeLow->GetValue().ToDouble( &value ) )
  {
    BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
    double* range = bp->GetDrawRange();
    bp->SetDrawRange( value, range[1] );
    UpdateTools();
  }
}

void ToolWindowEdit::OnEditDrawRangeHigh( wxCommandEvent& event )
{
  double value;
  if ( m_editDrawRangeHigh->GetValue().ToDouble( &value ) )
  {
    BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
    double* range = bp->GetDrawRange();
    bp->SetDrawRange( range[0], value );
    UpdateTools();
  }
}

void ToolWindowEdit::OnEditExcludeRangeLow( wxCommandEvent& event )
{
  double value;
  if ( m_editExcludeRangeLow->GetValue().ToDouble( &value ) )
  {
    BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
    double* range = bp->GetExcludeRange();
    bp->SetExcludeRange( value, range[1] );
    UpdateTools();
  }
}

void ToolWindowEdit::OnEditExcludeRangeHigh( wxCommandEvent& event )
{
  double value;
  if ( m_editExcludeRangeHigh->GetValue().ToDouble( &value ) )
  {
    BrushProperty* bp = MainWindow::GetMainWindowPointer()->GetBrushProperty();
    double* range = bp->GetExcludeRange();
    bp->SetExcludeRange( range[0], value );
    UpdateTools();
  }
}
