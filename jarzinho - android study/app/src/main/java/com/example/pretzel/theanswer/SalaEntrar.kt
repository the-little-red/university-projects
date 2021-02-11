package com.example.pretzel.theanswer

import android.os.Bundle
import android.app.Activity
import android.content.Intent
import android.view.View
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase


import kotlinx.android.synthetic.main.activity_sala_entrar.*
import kotlinx.android.synthetic.main.activity_sala_espera.*

class SalaEntrar : Activity() {
    lateinit var mDatabase: DatabaseReference
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_sala_entrar)
        mDatabase = FirebaseDatabase.getInstance().reference
    }
    fun Voltar(view: View){
        val sIntent = Intent (this, MainActivity::class.java).apply {

        }
        startActivity(sIntent)

    }

    fun EntraSala(view: View){
        val Player = Player.create()
        Player.Nome = nome2.text.toString()
        Player.SalaId = salaEditText.text.toString()
//        Player.Icone  = findViewById<View>(R.id.fab) as FloatingActionButton

        val novoPlayer = mDatabase.child(Player.SalaId.toString()).push()

        Player.PlayerId = novoPlayer.key

        novoPlayer.setValue(Player)

        val sIntent = Intent (this, SalaEspera::class.java).apply {
        }
        startActivity(sIntent);

    }

}
