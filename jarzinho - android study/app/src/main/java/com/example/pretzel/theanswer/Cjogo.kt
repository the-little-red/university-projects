package com.example.pretzel.theanswer

import android.os.Bundle
import android.app.Activity
import android.content.Intent
import android.view.View
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase
import kotlinx.android.synthetic.main.activity_cjogo.*
import kotlinx.android.synthetic.main.activity_sala_espera.*


class Cjogo : Activity() {
    lateinit var mDatabase: DatabaseReference

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_cjogo)
        mDatabase = FirebaseDatabase.getInstance().reference
    }

    fun Voltar(view: View){
        val sIntent = Intent (this, MainActivity::class.java).apply {

        }
        startActivity(sIntent);

    }

    fun CriaSala(view: View){

        val Player = Player.create()
        Player.Nome = nome1.text.toString()
//        Player.Icone  = findViewById<View>(R.id.fab) as FloatingActionButton

        Player.SalaId = Constants.FIREBASE_SALA + INDICE.toString()
        val novoPlayer = mDatabase.child(Player.SalaId.toString()).push()
        salaText.text = Player.SalaId.toString()
        nomeEditText.text = Player.Nome.toString()
        INDICE++
        Player.PlayerId = novoPlayer.key
        Player.Icone = ave.toString()
        novoPlayer.setValue(Player)

        val sIntent = Intent (this, SalaEspera::class.java).apply {
        }
        startActivity(sIntent);
    }
}
