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

// Qt includes
#include <QtPlugin>

// ModelMirrorPlus Logic includes
#include <vtkSlicerModelMirrorPlusLogic.h>

// ModelMirrorPlus includes
#include "qSlicerModelMirrorPlusModule.h"
#include "qSlicerModelMirrorPlusModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerModelMirrorPlusModule, qSlicerModelMirrorPlusModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerModelMirrorPlusModulePrivate
{
public:
  qSlicerModelMirrorPlusModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerModelMirrorPlusModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerModelMirrorPlusModulePrivate::qSlicerModelMirrorPlusModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerModelMirrorPlusModule methods

//-----------------------------------------------------------------------------
qSlicerModelMirrorPlusModule::qSlicerModelMirrorPlusModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerModelMirrorPlusModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerModelMirrorPlusModule::~qSlicerModelMirrorPlusModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerModelMirrorPlusModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerModelMirrorPlusModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerModelMirrorPlusModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerModelMirrorPlusModule::icon() const
{
  return QIcon(":/Icons/ModelMirrorPlus.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerModelMirrorPlusModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerModelMirrorPlusModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerModelMirrorPlusModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerModelMirrorPlusModule
::createWidgetRepresentation()
{
  return new qSlicerModelMirrorPlusModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerModelMirrorPlusModule::createLogic()
{
  return vtkSlicerModelMirrorPlusLogic::New();
}
