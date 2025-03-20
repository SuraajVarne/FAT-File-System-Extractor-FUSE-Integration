from kafka import KafkaConsumer
consumer = KafkaConsumer('fat32_requests', bootstrap_servers='localhost:9092')

for message in consumer:
    print(f"Received: {message.value}")
