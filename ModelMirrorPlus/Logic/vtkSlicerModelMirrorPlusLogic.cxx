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

// ModelMirrorPlus Logic includes
#include "vtkSlicerModelMirrorPlusLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerModelMirrorPlusLogic);

//----------------------------------------------------------------------------
vtkSlicerModelMirrorPlusLogic::vtkSlicerModelMirrorPlusLogic()
{
  this->MirroringPlane = XMIN;
  this->MergeMirrorModel = 0;
  this->CustomMirrorPlaneNormal[0] = 0.0;
  this->CustomMirrorPlaneNormal[1] = 0.0;
  this->CustomMirrorPlaneNormal[2] = 0.0;  
}

//----------------------------------------------------------------------------
vtkSlicerModelMirrorPlusLogic::~vtkSlicerModelMirrorPlusLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic
::MirrorModel(vtkMRMLModelNode* inputModel)
{
  if (!inputModel)
    {
    return;
    }

  this->MirrorModel(inputModel, inputModel);
}

//---------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic
::MirrorModel(vtkMRMLModelNode* inputModel, vtkMRMLModelNode* outputModel)
{
  if (!inputModel || !inputModel->GetPolyData())
    {
    return;
    }

  // ReflectionFilter is limited to X,Y,Z axis.
  // We need to define our own method for custom planes
  if (this->GetMirroringPlane() == CUSTOM)
    {
    this->MirrorModelCustomPlane(inputModel,outputModel);
    return;
    }

  // If mirroring along X,Y or Z axis, we can use ReflectionFilter
  vtkSmartPointer< vtkReflectionFilter > reflectionFilter = 
    vtkSmartPointer< vtkReflectionFilter >::New();
  reflectionFilter->SetInputData(inputModel->GetPolyData());
  reflectionFilter->CopyInputOff();

  if (this->GetMergeMirrorModel())
    {
    reflectionFilter->CopyInputOn();
    }
  
  switch(this->GetMirroringPlane())
    {
    case XMIN:
      reflectionFilter->SetPlaneToXMin();
      break;
    case XMAX:
      reflectionFilter->SetPlaneToXMax();
      break;
    case YMIN:
      reflectionFilter->SetPlaneToYMin();
      break;
    case YMAX:
      reflectionFilter->SetPlaneToYMax();
      break;
    case ZMIN:
      reflectionFilter->SetPlaneToZMin();
      break;
    case ZMAX:
      reflectionFilter->SetPlaneToZMax();
      break;
    default:
      reflectionFilter->SetPlaneToXMin();
      break;
    }
  reflectionFilter->Update();

  vtkSmartPointer< vtkDataSetSurfaceFilter > surfaceFilter = 
    vtkSmartPointer< vtkDataSetSurfaceFilter >::New();
  surfaceFilter->SetInputData(reflectionFilter->GetOutput());
  surfaceFilter->Update();

  outputModel->SetAndObservePolyData(surfaceFilter->GetOutput());
  outputModel->Modified();
}

//---------------------------------------------------------------------------
void vtkSlicerModelMirrorPlusLogic
::MirrorModelCustomPlane(vtkMRMLModelNode* inputModel, vtkMRMLModelNode* outputModel)
{  
  if (!inputModel || !outputModel)
    {
    return;
    }

  // Get plane normal
}
