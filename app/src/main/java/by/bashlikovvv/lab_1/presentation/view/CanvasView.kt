package by.bashlikovvv.lab_1.presentation.view

import android.content.Context
import android.graphics.Canvas
import android.util.AttributeSet
import android.view.View

class CanvasView : View {

    constructor(context: Context) : super(context)
    constructor(context: Context, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context, attrs: AttributeSet?, defStyleAttr: Int)
            : super(context, attrs, defStyleAttr)


    init {
        prepareEnvironment(getFilename())
    }

    override fun dispatchDraw(canvas: Canvas) {
        onPaintCanvas(canvas)
    }

    private external fun onPaintCanvas(canvas: Canvas)

    private external fun prepareEnvironment(pathToFile: String)

    private fun getFilename(): String {
        val dataDir = context.applicationContext.dataDir
        return dataDir.path + "/files/teapot.obj"
    }

}