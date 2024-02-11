package by.bashlikovvv.lab_1.model;

public class Point {

    private Vec4f _vertex;

    private int _textureInd = -1;

    private Vec3f _normal = new Vec3f(-1.1111, -1, -1);

    public Point(Vec4f _vertex, Vec3f _normal) {
        this._vertex = _vertex;
        this._normal = _normal;
    }

    public Vec4f get_vertex() {
        return _vertex;
    }

    public void set_vertex(Vec4f _vertex) {
        this._vertex = _vertex;
    }

    public int get_textureInd() {
        return _textureInd;
    }

    public void set_textureInd(int _textureInd) {
        this._textureInd = _textureInd;
    }

    public Vec3f get_normal() {
        return _normal;
    }

    public void set_normal(Vec3f _normal) {
        this._normal = _normal;
    }
}
