package by.bashlikovvv.lab_1.model;

public class Vec4f {

    private double[] _points = new double[4];

    public Vec4f(double v, double x, double y, double z) {
        this._points[0] = v;
        this._points[1] = x;
        this._points[2] = y;
        this._points[3] = z;
    }

    public double[] getPoints() {
        return _points;
    }

    public void setPoints(double[] points) {
        this._points = points;
    }
}
