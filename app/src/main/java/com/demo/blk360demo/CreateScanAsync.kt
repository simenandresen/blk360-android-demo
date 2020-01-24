package com.demo.blk360demo

import android.os.AsyncTask


class CreateScanAsync(val onFinished: (String) -> Unit) : AsyncTask<String, Unit, String>() {

    override fun doInBackground(vararg p0: String?): String {
        val response = createScan()
        return response
    }

    override fun onPostExecute(result: String?) {
        super.onPostExecute(result)
        this.onFinished(result!!)
    }

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("blk360wrapper-lib")
        }
    }

    external fun createScan(): String
}
