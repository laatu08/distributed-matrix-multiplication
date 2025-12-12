use serde::{Deserialize, Serialize};
use std::io::{BufRead, BufReader, Write};
use std::net::TcpStream;

#[derive(Serialize, Deserialize, Debug)]
struct MatrixPayload {
    a_rows: Vec<Vec<i32>>,
    b: Vec<Vec<i32>>,
}

fn multiply(a: Vec<Vec<i32>>, b: Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    let n = b[0].len();
    let mut result = vec![vec![0; n]; a.len()];

    println!("\nStarting matrix multiplication...");
    for i in 0..a.len() {
        for j in 0..n {
            let mut sum = 0;
            (0..b.len()).for_each(|k| {
                let product = a[i][k] * b[k][j];
                println!(
                    "a[{}][{}] * b[{}][{}] = {} * {} = {}",
                    i, k, k, j, a[i][k], b[k][j], product
                );
                sum += product;
            });
            result[i][j] = sum;
            println!("result[{i}][{j}] = {sum}\n");
        }
    }

    result
}

fn main() {
    const PORT: i32 = 8080;
    let mut stream = TcpStream::connect(format!("127.0.0.1:{PORT}")).unwrap();
    println!("Connected to server");

    let mut reader = BufReader::new(stream.try_clone().unwrap());
    let mut buffer = String::new();
    reader.read_line(&mut buffer).unwrap();

    // parse and show received data
    let payload: MatrixPayload = serde_json::from_str(&buffer).unwrap();
    println!("\nReceived payload from server:");
    println!(
        "Matrix A rows assigned to this client: {:?}",
        payload.a_rows
    );
    println!("Matrix B (full): {:?}", payload.b);

    // multiplication with trace
    let result = multiply(payload.a_rows, payload.b);

    // final result from client
    println!("\nComputed result by this client:");
    for row in &result {
        println!("{row:?}");
    }

    // send result back
    let serialized = serde_json::to_string(&result).unwrap();
    stream.write_all(serialized.as_bytes()).unwrap();
    stream.write_all(b"\n").unwrap();
    println!("\nSent result back to server.");
}
