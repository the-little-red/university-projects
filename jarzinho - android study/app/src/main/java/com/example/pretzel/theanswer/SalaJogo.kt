package com.example.pretzel.theanswer

import android.os.Bundle
import android.app.Activity
import android.content.Intent
import android.os.CountDownTimer
import android.view.View


import kotlinx.android.synthetic.main.activity_sala_jogo.*
import kotlin.concurrent.timer

var hide = false
class SalaJogo : Activity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_sala_jogo)
        val timer = MyCounter(500000, 1000)
        timer.start()
        local.text = "Praia"
        papel.text = "Turista"
    }

    fun EncerrarJogo(view: View){
        val sIntent = Intent (this, SalaEspera::class.java).apply {

        }
        startActivity(sIntent)

    }

    fun Esconde(view: View){
        if (hide == false) {
            local.visibility = View.GONE
            papel.visibility = View.GONE
            hide = true
        }
        if (hide == true) {
            local.visibility = View.VISIBLE
            papel.visibility = View.VISIBLE
            hide = false
        }
    }
    inner class MyCounter(millisInFuture: Long, countDownInterval: Long) : CountDownTimer(millisInFuture, countDownInterval) {

        override fun onFinish() {
            println("Timer Completed.")
            time.text = "Timer Completed."
        }

        override fun onTick(millisUntilFinished: Long) {
            time.textSize = 50f

            time.text = (millisUntilFinished / 1000).toString() + ""
        }


    }
}
