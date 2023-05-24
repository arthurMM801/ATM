# ATM

Nesse projeto, sera representado uma rede de comunicação entre terminais bancários (ATM) e um servidor usando a linguagem C:

1. O servidor são representados por um processo pai e os terminais ATM serão processos filhos;
2. Os terminais se comunicam com o servidor utilizando pipe;
3. Os terminais podem solicitar operações de saques e depósitos;
4. As informações de saldo de cada conta é controlada pelo servidor;
5. As instruções de saldo e saque de cada conta, em cada terminal deverá ser lida por um determinado aquivo;
6. Foram criados 4 terminais e 6 contas bancárias com saldo inicial igual 0;
7. Não são permitidos saques em contas com saldo menor que zero;
8. Ao fim do processo, o saldo bancário de todas as contas será mostrado;
