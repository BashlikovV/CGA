package by.bashlikovvv.lab_1.model;

public class Vec3f {

    private double[] _points = new double[3];

    public Vec3f(double i, double j, double k) {
        _points[0] = i;
        _points[1] = j;
        _points[2] = k;
    }

    public double[] getPoints() {
        return _points;
    }

    public void setPoints(double[] _points) {
        this._points = _points;
    }
}
