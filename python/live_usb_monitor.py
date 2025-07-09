from rich.live import Live
from rich.table import Table
from rich.console import Console
from rich.panel import Panel
from rich import box
import serial
import time

PORT = 'COM4'  # Change this to your serial port
BAUD = 115200

console = Console()
log = []

def build_table(log):
    table = Table(title="🛡️ DuckyFence Live USB Monitor", box=box.SIMPLE_HEAVY)
    table.add_column("Time", width=16)
    table.add_column("Event", style="cyan", no_wrap=True)

    for row in log[-20:]:
        table.add_row(*row)
    return table

def main():
    with serial.Serial(PORT, BAUD, timeout=1) as ser, Live(refresh_per_second=5) as live:
        while True:
            line = ser.readline().decode(errors='ignore').strip()
            if line:
                timestamp = time.strftime("%H:%M:%S")
                style = "bold red" if "ALERT" in line or "⚠️" in line else "white"
                log.append((timestamp, f"[{style}]{line}[/]"))
                live.update(build_table(log))

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        console.print("\n[bold red]Stopped by user.[/bold red]")
