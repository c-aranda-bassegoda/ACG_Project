#include "mainwindow.h"

#include "initialization/meshinitializer.h"
#include "initialization/objfile.h"
#include "subdivision/catmullclarksubdivider.h"
#include "subdivision/subdivider.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow Creates a new Main Window UI.
 * @param parent Qt parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->MeshGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
  ui->tessSettingsGroupBox->setEnabled(
      ui->MainDisplay->settings.tesselationMode);
}

/**
 * @brief MainWindow::~MainWindow Deconstructs the main window.
 */
MainWindow::~MainWindow() {
  delete ui;

  meshes.clear();
  meshes.squeeze();
}

/**
 * @brief MainWindow::importOBJ Imports an obj file and adds the constructed
 * half-edge to the collection of meshes.
 * @param fileName Path of the .obj file.
 */
void MainWindow::importOBJ(const QString& fileName) {
  OBJFile newModel = OBJFile(fileName);
  meshes.clear();
  meshes.squeeze();

  if (newModel.loadedSuccessfully()) {
    MeshInitializer meshInitializer;
    meshes.append(meshInitializer.constructHalfEdgeMesh(newModel));
    ui->MainDisplay->updateBuffers(meshes[0]);
    ui->MainDisplay->settings.modelLoaded = true;
  } else {
    ui->MainDisplay->settings.modelLoaded = false;
  }

  ui->MeshGroupBox->setEnabled(ui->MainDisplay->settings.modelLoaded);
  ui->SubdivSteps->setValue(0);
  ui->SharpSteps->setValue(0);
  ui->MainDisplay->update();
}

// Don't worry about adding documentation for the UI-related functions.

void MainWindow::on_LoadOBJ_pressed() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Import OBJ File", "../", tr("Obj Files (*.obj)"));
  importOBJ(filename);
}

void MainWindow::on_MeshPresetComboBox_currentTextChanged(
    const QString& meshName) {
  importOBJ(":/models/" + meshName + ".obj");
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
  ui->MainDisplay->settings.subdivSteps = value;
  Subdivider* subdivider = new CatmullClarkSubdivider();
  for (int k = meshes.size() - 1; k < value; k++) {
    meshes.append(subdivider->subdivide(meshes[k]));
  }
  ui->MainDisplay->updateBuffers(meshes[value]);
  delete subdivider;
  qDebug() << "SubdivSteps value changed\n";
}

void MainWindow::on_SharpSteps_valueChanged(double value) {
  QVector<HalfEdge>& halfedges = meshes[0].getHalfEdges();
  for (int i = 0; i < 3; i++){
    halfedges[i].setSharpness(value);
  }
    halfedges[3].setSharpness(value+1);

  qDebug() << "SharpnessLevel value changed\n";

  Subdivider* subdivider = new CatmullClarkSubdivider();
  for (int k = 1; k <= ui->MainDisplay->settings.subdivSteps; k++) {
      meshes[k] = subdivider->subdivide(meshes[k - 1]);
  }
  ui->MainDisplay->updateBuffers(meshes[ui->MainDisplay->settings.subdivSteps]);
  delete subdivider;
}

void MainWindow::on_TessellationCheckBox_toggled(bool checked) {
  ui->MainDisplay->settings.tesselationMode = checked;
  ui->tessSettingsGroupBox->setEnabled(checked);
  ui->MainDisplay->settings.uniformUpdateRequired = true;
  ui->MainDisplay->update();
}

void MainWindow::on_HideMeshCheckBox_toggled(bool checked) {
  // Useful for clearly seeing only the patches rendered by the Tessellation
  // shaders.
  ui->MainDisplay->settings.showCpuMesh = !checked;
  ui->MainDisplay->settings.uniformUpdateRequired = true;
  ui->MainDisplay->update();
}
