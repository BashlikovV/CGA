package by.bashlikovvv.lab_1.presentation

import android.app.Application

class App : Application() {

    companion object {
        init {
            System.loadLibrary("lab_1")
        }
    }

}