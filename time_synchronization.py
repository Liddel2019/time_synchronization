import asyncio
import serial_asyncio
import time

async def send_and_receive():
    reader, writer = await serial_asyncio.open_serial_connection(url='COM11', baudrate=115200)

    # Отправка текущего времени
    current_time = time.strftime('%Y-%m-%d %H:%M:%S')
    print(f"Отправляемое время: {current_time}")
    writer.write(current_time.encode())
    await writer.drain()

    # Чтение данных от Arduino без лишнего ожидания
    for _ in range(10):
        if not reader.at_eof():
            data = await reader.readline()
            print(f"Полученные данные: {data.decode().rstrip()}")
            current_time = time.strftime('%Y-%m-%d %H:%M:%S')
            print(f"Реальное время: {current_time}")

    writer.close()
    await writer.wait_closed()

asyncio.run(send_and_receive())