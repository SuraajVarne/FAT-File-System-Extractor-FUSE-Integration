use std::fs::File;
use std::io::{Read, Seek, SeekFrom};

fn read_fat32_header(file_path: &str) -> std::io::Result<()> {
    let mut file = File::open(file_path)?;
    let mut buffer = [0; 512];
    file.read_exact(&mut buffer)?;

    println!("OEM Name: {:?}", &buffer[3..11]);
    println!("Bytes Per Sector: {}", u16::from_le_bytes([buffer[11], buffer[12]]));
    println!("Sectors Per Cluster: {}", buffer[13]);
    println!("Reserved Sectors: {}", u16::from_le_bytes([buffer[14], buffer[15]]));

    Ok(())
}

fn main() {
    match read_fat32_header("disk.img") {
        Ok(_) => println!("FAT32 parsed successfully."),
        Err(e) => eprintln!("Error: {}", e),
    }
}
