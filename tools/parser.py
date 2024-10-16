import requests
import re
from bs4 import BeautifulSoup

url = "https://www.nvidia.com/en-us/drivers/unix/linux-amd64-display-archive/"
res = requests.get(url)

soup = BeautifulSoup(res.text, 'html.parser')
# Получение ссылок
search = soup.find("div", id="pressRoom")
kek = search.find_all("p")

links = "https://us.download.nvidia.com/XFree86/Linux-x86_64/560.35.03NVIDIA-Linux-x86_64-560.35.03.run"

# Получение версий
for i in kek:
    a = re.search("\d*\.\d*\.?\d+", i.text)
    if a:
        print(f"https://us.download.nvidia.com/XFree86/Linux-x86_64/{a.group(0)}/NVIDIA-Linux-x86_64-{a.group(0)}.run")

