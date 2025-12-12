use serde::{Deserialize, Serialize};
use std::io::{BufRead, BufReader, Write};
use std::net::{TcpListener, TcpStream};
use std::thread;

#[derive(Serialize, Deserialize, Debug)]
struct MatrixPayload {
    a_rows: Vec<Vec<i32>>, // assigned rows of A
    b: Vec<Vec<i32>>,      // full matrix B
}

fn handle_client(mut stream: TcpStream, a_rows: Vec<Vec<i32>>, b: Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    let payload = MatrixPayload { a_rows, b };
    let serialized = serde_json::to_string(&payload).unwrap();
    stream.write_all(serialized.as_bytes()).unwrap();
    stream.write_all(b"\n").unwrap(); // simple delimiter

    // Receive result
    let mut buffer = String::new();
    let mut reader = BufReader::new(&stream);
    reader.read_line(&mut buffer).unwrap();
    let result: Vec<Vec<i32>> = serde_json::from_str(buffer.trim()).unwrap();
    result
}

fn multiply_distributed() {
    const PORT: i32 = 8080;
    let listener = TcpListener::bind(format!("127.0.0.1:{PORT}")).unwrap();
    println!("Server listening on port {PORT}");

    // square matrices
    let a = [
        vec![1, 2, 3, 4],
        vec![5, 6, 7, 8],
        vec![9, 10, 11, 12],
        vec![13, 14, 15, 16],
    ];
    let b = vec![
        vec![1, 0, 0, 0],
        vec![0, 1, 0, 0],
        vec![0, 0, 1, 0],
        vec![0, 0, 0, 1],
    ];

    // split A into two parts
    let mid = a.len() / 2;
    let a1 = a[..mid].to_vec();
    let a2 = a[mid..].to_vec();

    let mut handles = vec![];
    let mut results: [Option<Vec<Vec<i32>>>; 2] = [None, None];

    for (i, a_part) in [a1, a2].into_iter().enumerate() {
        let b_clone = b.clone();
        let stream = listener.accept().unwrap().0;
        println!("Client {} connected", i + 1);

        let handle = thread::spawn(move || handle_client(stream, a_part, b_clone));
        handles.push((i, handle));
    }

    for (i, handle) in handles {
        let client_result = handle.join().unwrap();
        println!("Result from Client {}:", i + 1);
        for row in &client_result {
            println!("{row:?}");
        }
        results[i] = Some(client_result);
    }

    let final_result: Vec<Vec<i32>> = results.into_iter().flat_map(|r| r.unwrap()).collect();

    println!("Final Result:");
    for row in final_result {
        println!("{row:?}");
    }
}

fn main() {
    multiply_distributed();
}
