package com.bytebitx.ndkdemo

import android.annotation.SuppressLint
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.bytebitx.ndkdemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private val TAG = "MainActivity"

    private lateinit var binding: ActivityMainBinding

    private var basicL: Long = 0L
    private var basicB: Boolean = false
    private var basicS: String = "normal s"

    companion object {
        var sBasicL: Long = 1L
        var sBasicB: Boolean = true
        var sBasicS: String = "s normal s"

        external fun staticAccessNormalL(l: Long): Long
        init {
            System.loadLibrary("main")
        }
    }

    private val testStringBuilder = StringBuilder()

    private val testCallback: TestCallBack = object : TestCallBack {
        override fun onVideoFrame(frames: Int, bitrateKbps: Long) {
            Log.d(TAG, "onVideoFrame2 : $frames, $bitrateKbps")
            testStringBuilder.append(frames).append(",")
                .append(bitrateKbps).append(",")
        }

        override fun onConnectSuccess(width: Int, height: Int) {
            Log.d(TAG, "onConnectSuccess2 : $width, $height")
            testStringBuilder.append(width).append(",")
                .append(height).append(",")
        }

        override fun onConnectError() {
            Log.d(TAG, "onConnectError2")
            testStringBuilder.append("onConnectError")
        }
    }

    @SuppressLint("SetTextI18n")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)


        binding.btnNormalBasicVar.setOnClickListener {
            val list = accessBasicVar(12, true, "this is java string")
            binding.tvBasicStatic.text = "$basicL,$basicB,$basicS,$sBasicL,$sBasicB,$sBasicS\n"
            binding.tvBasicStaticList.text = "$list\n"
        }
        binding.btnReference.setOnClickListener {
            accessReferenceVar(testCallback)
            binding.tvRefResult.text = testStringBuilder.toString()
        }
        binding.btnMainInstance.setOnClickListener {
            accessMainInstance(this)
        }
        binding.btnStatic.setOnClickListener {
            staticAccessNormalL(1L)
            binding.btnStatic.text = sBasicL.toString()
        }

        binding.btnThreadCallBack.setOnClickListener {
            accessCallBackByChildThread(object : TestCallBack{
                override fun onVideoFrame(frames: Int, bitrateKbps: Long) {
                    val threadName = Thread.currentThread().name
                    runOnUiThread {
                        binding.btnThreadCallBack.text = "current thread is : $threadName , frames: $frames, bitrateKbps: $bitrateKbps"
                    }
                }

                override fun onConnectSuccess(width: Int, height: Int) {
                }

                override fun onConnectError() {
                }

            })
        }

        binding.btnReferenceManage.setOnClickListener {
            val result = accessGlobalReference()
            Log.d(TAG, "result : $result")
        }

        binding.sampleText.text = getFFmpegVersion()
    }

    @SuppressLint("SetTextI18n")
    override fun onResume() {
        super.onResume()
        val content = binding.tvMainInstance.text.toString()
        if (!content.contains("success")) {
            binding.tvMainInstance.text = "$content success"
        } else {
            binding.tvMainInstance.text = "call main onResume success"
        }

    }

    class RealPlayCallBack : TestCallBack {
        override fun onVideoFrame(frames: Int, bitrateKbps: Long) {
            Log.d("MainActivity", "onVideoFrame : $frames, $bitrateKbps")
        }

        override fun onConnectSuccess(width: Int, height: Int) {
            Log.d("MainActivity", "onConnectSuccess : $width, $height")
        }

        override fun onConnectError() {
            Log.d("MainActivity", "onConnectError")
        }
    }


    interface TestCallBack {
        fun onVideoFrame(frames: Int, bitrateKbps: Long)
        fun onConnectSuccess(width: Int, height: Int)
        fun onConnectError()
    }

    // 基础数据类型
    external fun accessBasicVar(l: Long, b: Boolean, s: String): ArrayList<Any>
    // 引用类型
    external fun accessReferenceVar(testCallBack: TestCallBack)
    // 引用类型，传递MainActivity
    external fun accessMainInstance(mainActivity: MainActivity)
    // 子线程回调callback方法
    external fun accessCallBackByChildThread(testCallBack: TestCallBack)
    // 局部引用
    external fun accessLocalReference(): String
    // 全局引用
    external fun accessGlobalReference(): String
    // 弱引用
    external fun accessWeakReference()
    // 获取ffmpeg版本
    external fun getFFmpegVersion(): String

}