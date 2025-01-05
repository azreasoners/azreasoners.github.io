<?

$json = json_decode(file_get_contents('php://input'));
$request = $json->request_type;

// Deep-reasoning server
//$url = "http://10.218.107.187:5000/";

// Cogitari server
// $url = "http://10.218.104.140:5000/";
$url = "http://en4113966l.cidse.dhcp.asu.edu:5000/";



if($request == "example"){
$data = array(
            "request_type" => "example",
            "type" => $json->type,
            "instance" =>  $json->instance,
        );
}
if($request == "tutorial"){
$data = array(
            "request_type" => "tutorial",
            "type" => $json->type,
            "instance" =>  $json->instance,
        );
}
else if ($request == "inference"){
    $data = array(
                "request_type" => "inference",
                "command" => $json->command,
                "content" =>  $json->content,
                "evidence" =>  $json->evidence,
                "session"=>md5(uniqid(rand(), true)),
            );
}
else if ($request == "learn"){
    $data = array(
                "request_type" => "learn",
                "command" => $json->command,
                "content" =>  $json->content,
                "observation" =>  $json->observation,
                "session"=>md5(uniqid(rand(), true)),
            );
}
else if ($request == "dt"){
    $data = array(
                "request_type" => "inference",
                "command" => $json->command,
                "content" =>  $json->content,
                "query" =>  $json->query,
                "session"=>md5(uniqid(rand(), true)),
            );
}
$options = array(
    'http' => array(
    'method'  => 'POST',
    'content' => json_encode( $data ),
    'header'=>  "Content-Type: application/json\r\n" .
                "Accept: application/json\r\n"
    )
);


$context  = stream_context_create( $options );
$result = file_get_contents( $url . $request, false, $context );
echo $result;
?>