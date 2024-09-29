<?php
$servername = "localhost"; // Nome do servidor, geralmente 'localhost' para XAMPP
$username = "root";         // Nome de usuário padrão do XAMPP
$password = "";             // Senha padrão do MySQL (geralmente em branco no XAMPP)
$dbname = "projete_oficial"; // Nome do seu banco de dados

// Criar conexão com o banco de dados
$conn = new mysqli($servername, $username, $password, $dbname);

// Verifica se há erro na conexão
if ($conn->connect_error) {
    die("Conexão falhou: " . $conn->connect_error);
}

// Coleta os parâmetros de URL
$red = isset($_GET['red']) ? intval($_GET['red']) : 0;     // Valor do LED vermelho
$green = isset($_GET['green']) ? intval($_GET['green']) : 0; // Valor do LED verde
$yellow = isset($_GET['yellow']) ? intval($_GET['yellow']) : 0; // Valor do LED amarelo

// Insere os dados na tabela
$sql = "INSERT INTO led_status (red, green, yellow, timestamp) VALUES ($red, $green, $yellow, CURRENT_TIMESTAMP())";

if ($conn->query($sql) === TRUE) {
    echo "Dados inseridos com sucesso!";
} else {
    echo "Erro ao inserir dados: " . $conn->error; // Exibe o erro se a inserção falhar
}

$conn->close(); // Fecha a conexão com o banco de dados
?>
