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

// .NAME vtkSlicerModelMirrorPlusLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerModelMirrorPlusLogic_h
#define __vtkSlicerModelMirrorPlusLogic_h

// VTK includes
#include "vtkDataSetSurfaceFilter.h"
#include "vtkPolyData.h"
#include "vtkReflectionFilter.h"

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes
#include "vtkMRMLLinearTransformNode.h"
#include "vtkMRMLModelNode.h"

// STD includes
#include <cstdlib>

#include "vtkSlicerModelMirrorPlusModuleLogicExport.h"


/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_MODELMIRRORPLUS_MODULE_LOGIC_EXPORT vtkSlicerModelMirrorPlusLogic :
  public vtkSlicerModuleLogic
{
public:

  enum {
    XMIN = 0,
    XMAX,
    YMIN,
    YMAX,
    ZMIN,
    ZMAX,
    CUSTOM
  };

  static vtkSlicerModelMirrorPlusLogic *New();
  vtkTypeMacro(vtkSlicerModelMirrorPlusLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetMacro(MirroringPlane,int);
  vtkGetMacro(MirroringPlane,int);
  vtkSetMacro(MergeMirrorModel,int);
  vtkGetMacro(MergeMirrorModel,int);
  vtkSetVector3Macro(CustomMirrorPlaneNormal,double);
  vtkGetVector3Macro(CustomMirrorPlaneNormal,double);
  
  void SetMirrorPlaneToXMin() { this->SetMirroringPlane(XMIN); }
  void SetMirrorPlaneToXMax() { this->SetMirroringPlane(XMAX); }
  void SetMirrorPlaneToYMin() { this->SetMirroringPlane(YMIN); }
  void SetMirrorPlaneToYMax() { this->SetMirroringPlane(YMAX); }
  void SetMirrorPlaneToZMin() { this->SetMirroringPlane(ZMIN); }
  void SetMirrorPlaneToZMax() { this->SetMirroringPlane(ZMAX); }
  void SetMirrorPlaneToCustom(double* mirroringPlaneNormal) 
  { 
    this->SetMirroringPlane(CUSTOM); 
    this->SetCustomMirrorPlaneNormal(mirroringPlaneNormal);
  }

  void MirrorModel(vtkMRMLModelNode* inputModel);
  void MirrorModel(vtkMRMLModelNode* inputModel, vtkMRMLModelNode* outputModel);
  void MirrorModelCustomPlane(vtkMRMLModelNode* inputModel, vtkMRMLModelNode* outputModel);

protected:
  vtkSlicerModelMirrorPlusLogic();
  virtual ~vtkSlicerModelMirrorPlusLogic();

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);
private:

  vtkSlicerModelMirrorPlusLogic(const vtkSlicerModelMirrorPlusLogic&); // Not implemented
  void operator=(const vtkSlicerModelMirrorPlusLogic&); // Not implemented

  int MirroringPlane;
  int MergeMirrorModel;
  double CustomMirrorPlaneNormal[3];
};

#endif
