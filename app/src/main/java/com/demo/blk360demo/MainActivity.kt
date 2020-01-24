package com.demo.blk360demo

import android.content.Context
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.PowerManager
import android.util.Log
import android.widget.Button
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    var wakeLock: PowerManager.WakeLock? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val button = findViewById<Button>(R.id.scanBtn)
        val status = findViewById<TextView>(R.id.textStatus)
        button.setOnClickListener {
            status.text = "Busy scanning..."
            val task = CreateScanAsync() {
                Log.i("demo", "Response: $it")
                status.text = it
            }
            task.execute()
        }
    }

    override fun onStart() {
        super.onStart()

        // Not sure if we need to use a wakeLock, but the idea is that it might be required so the activity
        // stays alive while scanning
        wakeLock =
            (getSystemService(Context.POWER_SERVICE) as PowerManager).run {
                newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "MyApp::MyWakelockTag").apply {
                    acquire(10 * 60 * 1000L /*10 minutes*/)
                }
            }
    }

    override fun onStop() {
        super.onStop()
        wakeLock?.release()
    }
}
