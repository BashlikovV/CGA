package by.bashlikovvv.lab_1.model;

public class Face {

    private Point[] _points = new Point[3];

    private Vec3f _normal;

    public Face(Point[] _points, Vec3f _normal) {
        this._points = _points;
        this._normal = _normal;
    }

    public Face() {  }
}
