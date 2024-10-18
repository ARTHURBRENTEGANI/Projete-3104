<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: GET, POST, OPTIONS");
header("Access-Control-Allow-Headers: Content-Type");
header("Content-Type: application/json");

$servername = "localhost";
$username = "root";
$password = "";
$dbname = "projete_oficial";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Conexão falhou: " . $conn->connect_error);
}

$red = isset($_GET['red']) ? intval($_GET['red']) : 0;
$green = isset($_GET['green']) ? intval($_GET['green']) : 0;
$yellow = isset($_GET['yellow']) ? intval($_GET['yellow']) : 0;

if (isset($_GET['red']) || isset($_GET['green']) || isset($_GET['yellow'])) {
    $sql_insert = "INSERT INTO led_status (red, green, yellow, timestamp) VALUES ($red, $green, $yellow, CURRENT_TIMESTAMP())";

    if ($conn->query($sql_insert) === TRUE) {
        echo json_encode(["message" => "Dados inseridos com sucesso!"]);
    } else {
        echo json_encode(["error" => "Erro ao inserir dados: " . $conn->error]);
    }
}

$sql_select = "SELECT red, green, yellow FROM led_status ORDER BY timestamp DESC LIMIT 1";
$result = $conn->query($sql_select);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    echo json_encode($row);
} else {
    echo json_encode(['error' => 'Nenhum dado encontrado', 'red' => 0, 'green' => 0, 'yellow' => 0]);
}

$conn->close();
?>
