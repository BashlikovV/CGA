package by.bashlikovvv.lab_1.data;

import by.bashlikovvv.lab_1.model.Face;

public class ObjFileParser {

    public native String getLibraryName();

    public native Face[] getStructuresFromFile(String filename);

}