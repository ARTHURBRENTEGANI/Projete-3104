<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: GET, POST, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type");
header("Content-Type: application/json");

$servername = "localhost"; // Nome do servidor
$username = "root";         // Nome de usuário
$password = "";             // Senha
$dbname = "projete_oficial"; // Nome do banco de dados

// 1. Cria uma conexão com o banco de dados
$conn = new mysqli($servername, $username, $password, $dbname);

// 2. Verifica se houve erro na conexão
if ($conn->connect_error) {
    die("Conexão falhou: " . $conn->connect_error);
}

// 3. Coleta os parâmetros de URL (se existirem)
$red = isset($_GET['red']) ? intval($_GET['red']) : 0;       // Valor do LED vermelho
$green = isset($_GET['green']) ? intval($_GET['green']) : 0; // Valor do LED verde
$yellow = isset($_GET['yellow']) ? intval($_GET['yellow']) : 0; // Valor do LED amarelo

// 4. Insere os dados recebidos na tabela (se os parâmetros existirem)
if (isset($_GET['red']) || isset($_GET['green']) || isset($_GET['yellow'])) {
    $sql_insert = "INSERT INTO led_status (red, green, yellow, timestamp) VALUES ($red, $green, $yellow, CURRENT_TIMESTAMP())";
    
    if ($conn->query($sql_insert) === TRUE) {
        echo json_encode(["message" => "Dados inseridos com sucesso!"]);
    } else {
        echo json_encode(["error" => "Erro ao inserir dados: " . $conn->error]); // Retorna erro em JSON
    }
}

// 5. Executa uma consulta SQL para buscar o estado mais recente dos LEDs
$sql_select = "SELECT red, green, yellow FROM led_status ORDER BY timestamp DESC LIMIT 1";
$result = $conn->query($sql_select);

// 6. Verifica se houve algum resultado da consulta
if ($result->num_rows > 0) {
    // 7. Converte o resultado em um array associativo e retorna como JSON
    $row = $result->fetch_assoc();
    echo json_encode($row);
} else {
    // 8. Se não houver dados, retorna uma mensagem de erro em JSON e valores padrão (0)
    echo json_encode(['error' => 'Nenhum dado encontrado', 'red' => 0, 'green' => 0, 'yellow' => 0]);
}

// 9. Fecha a conexão com o banco de dados
$conn->close();
?>
