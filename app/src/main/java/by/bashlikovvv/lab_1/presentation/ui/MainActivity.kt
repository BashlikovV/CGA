package by.bashlikovvv.lab_1.presentation.ui

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.gestures.rememberTransformableState
import androidx.compose.foundation.gestures.transformable
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.drawWithCache
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.drawscope.DrawScope
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.round
import androidx.compose.ui.viewinterop.AndroidView
import by.bashlikovvv.lab_1.R
import by.bashlikovvv.lab_1.presentation.view.CanvasView
import by.bashlikovvv.lab_1.ui.theme.Lab_1Theme
import kotlin.concurrent.thread
import kotlin.math.roundToInt
import kotlin.properties.Delegates

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            Lab_1Theme {
                ApplicationContent(
                    pathToFile = getFilename(),
                    onScale = { zoom -> thread { onScale(zoom) }  },
                    onRotate = { angle -> thread { onRotate(angle) } },
                    onTranslate = { offset -> thread { onTranslate(offset.x, offset.y) } },
                )
            }
        }
    }

    private external fun getCPPString(): String

    private external fun onPaint(drawScope: DrawScope, pathToFile: String)

    private external fun onTranslate(x: Float, y: Float)

    private external fun onRotate(angle: Float)

    private external fun onScale(zoom: Float)

    private fun getFilename(): String {
        val dataDir = applicationContext.dataDir
        return dataDir.path + "/files/teapot.obj"
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) {
            hideSystemUi()
        }
    }

    private fun hideSystemUi() {
        val decorView = window.decorView
        decorView.systemUiVisibility = (View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_FULLSCREEN)
    }

}

@Composable
fun ApplicationContent(
    pathToFile: String,
    onScale: (Float) -> Unit,
    onRotate: (Float) -> Unit,
    onTranslate: (Offset) -> Unit
) {
    var scaleState by Delegates.observable(1f) { _, oldValue, newValue ->
        if (oldValue != newValue) {
            onScale(newValue)
        }
    }
    var translationState by Delegates.observable(Offset(0f, 0f)) { _, oldValue, newValue ->
        if (oldValue.round() != newValue.round()) {
            onTranslate(newValue)
        }
    }
    var rotationState by Delegates.observable(0f) { _, oldValue, newValue ->
        if (oldValue.roundToInt() != newValue.roundToInt()) {
            onRotate(newValue)
        }
    }
    val state = rememberTransformableState { zoomChange, panChange, rotationChange ->
        scaleState *= zoomChange
        translationState += panChange
        rotationState += rotationChange
    }

    Text(text = pathToFile)

    AndroidView(
        factory = { context -> CanvasView(context) },
        modifier = Modifier
            .fillMaxSize()
            .transformable(state)
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    Lab_1Theme {
        ApplicationContent(
            pathToFile = "",
            onScale = { _ ->  },
            onRotate = { _ ->  },
            onTranslate = { _ ->  }
        )
    }
}