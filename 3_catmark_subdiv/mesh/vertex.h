#ifndef VERTEX
#define VERTEX

#include <QVector3D>

// Forward declaration
class HalfEdge;

/**
 * @brief The Vertex class represents a vertex within a half-edge mesh.
 */
class Vertex {
 public:
  Vertex();
  Vertex(QVector3D coords, HalfEdge* out, int valence, int index);

  HalfEdge* nextBoundaryHalfEdge() const;
  HalfEdge* prevBoundaryHalfEdge() const;
  QVector<Vertex*> getVerticesOfSharpEdges() const;
  double getAvgSharpness() const;
  bool isBoundaryVertex() const;
  void recalculateValence();
  void recalculateSharpIncidence();
  void debugInfo() const;

  QVector3D coords;
  HalfEdge* out;
  int valence = 0;
  int index;

  int incidentSharpEdges=0;
};

#endif  // VERTEX
