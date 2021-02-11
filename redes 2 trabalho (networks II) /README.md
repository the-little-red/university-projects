# Trabalho de Redes2 - Streaming em UDP - BCC


# Definição do Trabalho

    Neste trabalho você vai implementar um servidor de streams. Eles são cada vez mais comuns, por exemplo o Netflix tem servidores de stream de vídeo; bolsas de valores tem servidores atualizando cotações continuamente; agências metereológicas produzem streams de previsões.
    No caso do trabalho, cada dupla vai definir a natureza do stream, podendo inclusive ser multimídia (lembre-se que os trabalhos devem poder ser defendidos aqui no laboratório). Concretamente, o stream consiste de uma uma sequência de mensagens de um determinado tipo, com campos específicos. O número de campos é no mínimo 2, pois obrigatoriamente cada pacote informa sua ordem no stream, começando por 1. Deve ser possível configurar via linha de comando o intervalo de tempo em que cada mensagem do stream é transmitida.
    Vários clientes podem se registrar no servidor para receber o stream. A comunicação entre clientes e servidores é sobre UDP/IP. Ao encerrar o cliente ele deve produzir estatísticas sobre o uso do UDP: quantos pacotes foram perdidos e quantos chegaram fora de ordem.
    O cliente deve implementar uma operação sobre os dados recebidos. Esta operação é definida pela dupla tendo em vista o serviço oferecido. Por exemplo, esta operação pode retornar os dados mais populares ou menos populares, ou a soma de todos os dados recebidos, ou listar todos os pacotes para um determinado valor de campo Por exemplo, todas as previsões metereológicas para Curitiba.
    Devem ser apresentados logs para múltiplas execuções. Pelo menos uma das execuções deve ter obrigatoriamente 3 clientes recebendo o stream.

# O que foi implementado



# Como usar


# Alunos: Arianne Bortolan, Dante Aleo
