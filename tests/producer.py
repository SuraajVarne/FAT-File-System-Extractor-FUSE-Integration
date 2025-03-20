from kafka import KafkaProducer
producer = KafkaProducer(bootstrap_servers='localhost:9092')

producer.send('fat32_requests', b'Parse FAT32 disk image')
producer.flush()
