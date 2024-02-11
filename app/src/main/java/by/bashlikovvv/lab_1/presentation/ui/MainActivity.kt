package by.bashlikovvv.lab_1.presentation.ui

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.gestures.rememberTransformableState
import androidx.compose.foundation.gestures.transformable
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.drawscope.DrawScope
import androidx.compose.ui.tooling.preview.Preview
import by.bashlikovvv.lab_1.ui.theme.Lab_1Theme

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            Lab_1Theme {
                ApplicationContent(getFilename()) { drawScope, s, rotation, translation ->
                    onPaintWithTranslation(drawScope, s, translation.x, translation.y)
                }
            }
        }
    }

    private external fun getCPPString(): String

    private external fun onPaint(drawScope: DrawScope, pathToFile: String)

    private external fun onPaintWithTranslation(drawScope: DrawScope, pathToFile: String, x: Float, y: Float)

    private fun getFilename(): String {
        val dataDir = applicationContext.dataDir
        return dataDir.path + "/files/teapot.obj"
    }

}

@Composable
fun ApplicationContent(
    pathToFile: String,
    onPaint: (DrawScope, String, Float, Offset) -> Unit,
) {
    var scaleState by remember { mutableFloatStateOf(1f) }
    var translationState by remember { mutableStateOf(Offset(0f, 0f)) }
    var rotationState by remember { mutableFloatStateOf(0f) }
    val state = rememberTransformableState { zoomChange, panChange, rotationChange ->
        scaleState *= zoomChange
        translationState += panChange
        rotationState += rotationChange
    }

    Text(text = pathToFile)

    Canvas(
        modifier = Modifier
            .fillMaxSize()
            .transformable(state)
    ) { onPaint(this, pathToFile, rotationState, translationState) }
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    Lab_1Theme {
        ApplicationContent(onPaint = { scope, str, rotation, translation ->  }, pathToFile = "")
    }
}