/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// MRML includes
#include "vtkMRMLModelDisplayNode.h"
#include "vtkMRMLModelNode.h"
#include "vtkMRMLScene.h"

// VTK includes
#include "vtkPlaneWidget.h"
#include "vtkPolyData.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qMRMLThreeDView.h"
#include "qMRMLThreeDWidget.h"
#include "qSlicerApplication.h"
#include "qSlicerLayoutManager.h"
#include "qSlicerModelMirrorPlusModuleWidget.h"
#include "ui_qSlicerModelMirrorPlusModuleWidget.h"

#include "vtkSlicerModelMirrorPlusLogic.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerModelMirrorPlusModuleWidgetPrivate: public Ui_qSlicerModelMirrorPlusModuleWidget
{
public:
  qSlicerModelMirrorPlusModuleWidgetPrivate();
  ~qSlicerModelMirrorPlusModuleWidgetPrivate();

  vtkPlaneWidget* planeWidget;
};

//-----------------------------------------------------------------------------
// qSlicerModelMirrorPlusModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerModelMirrorPlusModuleWidgetPrivate::qSlicerModelMirrorPlusModuleWidgetPrivate()
{
  this->planeWidget = vtkPlaneWidget::New();
}

//-----------------------------------------------------------------------------
qSlicerModelMirrorPlusModuleWidgetPrivate::~qSlicerModelMirrorPlusModuleWidgetPrivate()
{
  this->planeWidget->Delete();
}

//-----------------------------------------------------------------------------
// qSlicerModelMirrorPlusModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerModelMirrorPlusModuleWidget::qSlicerModelMirrorPlusModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerModelMirrorPlusModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerModelMirrorPlusModuleWidget::~qSlicerModelMirrorPlusModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerModelMirrorPlusModuleWidget::setup()
{
  Q_D(qSlicerModelMirrorPlusModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  connect(d->XMinButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMirroringPlaneChanged()));
  connect(d->XMaxButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMirroringPlaneChanged()));
  connect(d->YMinButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMirroringPlaneChanged()));
  connect(d->YMaxButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMirroringPlaneChanged()));
  connect(d->ZMinButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMirroringPlaneChanged()));
  connect(d->ZMaxButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMirroringPlaneChanged()));
  connect(d->CustomPlaneButton, SIGNAL(toggled(bool)),
	  this, SLOT(onMirroringPlaneChanged()));

  connect(d->MergeMirrorInputCheckbox, SIGNAL(toggled(bool)),
	  this, SLOT(onMergeMirrorInputToggled(bool)));

  connect(d->ApplyButton, SIGNAL(clicked()),
	  this, SLOT(onApplyClicked()));
}

//-----------------------------------------------------------------------------
void qSlicerModelMirrorPlusModuleWidget::onMirroringPlaneChanged()
{
  Q_D(qSlicerModelMirrorPlusModuleWidget);

  // Get logic
  vtkSlicerModelMirrorPlusLogic* mmpLogic = vtkSlicerModelMirrorPlusLogic::SafeDownCast(this->logic());
  if (!mmpLogic)
    {
    return;
    }

  if (!d->CustomPlaneButton->isChecked() && d->planeWidget)
    {
    d->planeWidget->Off();
    }

  // Set Mirroring Plane
  if (d->XMinButton->isChecked())
    {
    mmpLogic->SetMirrorPlaneToXMin();
    }
  else if (d->XMaxButton->isChecked())
    {
    mmpLogic->SetMirrorPlaneToXMax();
    }
  else if (d->YMinButton->isChecked())
    {
    mmpLogic->SetMirrorPlaneToYMin();
    }
  else if (d->YMaxButton->isChecked())
    {
    mmpLogic->SetMirrorPlaneToYMax();
    }
  else if (d->ZMinButton->isChecked())
    {
    mmpLogic->SetMirrorPlaneToZMin();
    }
  else if (d->ZMaxButton->isChecked())
    {
    mmpLogic->SetMirrorPlaneToZMax();
    }
  else if (d->CustomPlaneButton->isChecked())
    {
    if (!d->planeWidget)
      {
      d->XMinButton->setChecked(true);
      return;
      }
    
    // Get 3D view interactor
    qSlicerApplication* app = qSlicerApplication::application();
    qSlicerLayoutManager* layoutManager = app->layoutManager();
    qMRMLThreeDWidget* threeDWidget = layoutManager->threeDWidget(0);
    qMRMLThreeDView* threeDView = threeDWidget->threeDView();
    vtkRenderWindowInteractor* interactor = threeDView->renderWindow()->GetInteractor();

    if (!interactor)
      {
      d->XMinButton->setChecked(true);
      return;      
      }

    if (!d->planeWidget->GetInteractor() || d->planeWidget->GetInteractor() != interactor)
      {
      d->planeWidget->SetInteractor(interactor);
      d->planeWidget->Off();
      d->planeWidget->SetPoint1(0,200,0);
      d->planeWidget->SetPoint2(0,0,200);
      d->planeWidget->SetCenter(0,0,0);
      d->planeWidget->SetHandleSize(0.01);
      d->planeWidget->SetRepresentationToWireframe();
      d->planeWidget->PlaceWidget();
      }
    d->planeWidget->On();
    }
}

//-----------------------------------------------------------------------------
void qSlicerModelMirrorPlusModuleWidget::onMergeMirrorInputToggled(bool merge)
{
  Q_D(qSlicerModelMirrorPlusModuleWidget);

  d->OutputModelSelector->setEnabled(!merge);

  // Get logic
  vtkSlicerModelMirrorPlusLogic* mmpLogic = vtkSlicerModelMirrorPlusLogic::SafeDownCast(this->logic());
  if (!mmpLogic)
    {
    return;
    }
  
  mmpLogic->SetMergeMirrorModel(merge ? 1 : 0);
}

//-----------------------------------------------------------------------------
void qSlicerModelMirrorPlusModuleWidget::onApplyClicked()
{
  Q_D(qSlicerModelMirrorPlusModuleWidget);

  vtkMRMLModelNode* inputModel = vtkMRMLModelNode::SafeDownCast(d->InputModelSelector->currentNode());
  vtkMRMLModelNode* outputModel = vtkMRMLModelNode::SafeDownCast(d->OutputModelSelector->currentNode());

  // Get logic
  vtkSlicerModelMirrorPlusLogic* mmpLogic = vtkSlicerModelMirrorPlusLogic::SafeDownCast(this->logic());
  if (!mmpLogic)
    {
    return;
    }

  if (d->CustomPlaneButton->isChecked() && d->planeWidget)
    {
    
    }

  // Merge mirror with input ?
  if (d->MergeMirrorInputCheckbox->isChecked())
    {
    if (!inputModel || !inputModel->GetPolyData())
      {
      return;
      }
    mmpLogic->MirrorModel(inputModel);
    }
  else
    {
    if (!inputModel || !outputModel)
      {
      return;
      }

    // Create model display node if not already existing
    if (!outputModel->GetDisplayNode())
      {
      vtkSmartPointer< vtkMRMLModelDisplayNode > displayNode =
	vtkSmartPointer< vtkMRMLModelDisplayNode >::New();
      this->mrmlScene()->AddNode(displayNode.GetPointer());
      outputModel->SetAndObserveDisplayNodeID(displayNode->GetID());
      }
    
    mmpLogic->MirrorModel(inputModel, outputModel);
    }
}
