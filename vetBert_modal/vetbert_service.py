import modal
from modal import App, Image, Volume

app = App("vetbert-service")

image = Image.debian_slim().pip_install_from_requirements("requirements.txt")
secrets = []  # Eğer Hugging Face private model indirilecekse ekle: [modal.Secret.from_name("hf-secret")]

BASE_MODEL = "havocy28/VetBERTDx"

CACHE_DIR = "/cache"
MIN_CONTAINERS = 0  # 1 yaparsan sıcak kalır ama kredi yer

hf_cache = Volume.from_name("hf-hub-cache", create_if_missing=True)

@app.cls(
    image=image.env({"HF_HUB_CACHE": CACHE_DIR}),
    secrets=secrets,
    gpu="T4",
    timeout=1800,
    volumes={CACHE_DIR: hf_cache},
    min_containers=MIN_CONTAINERS
)
class VetBERTClassifier:
    @modal.enter()
    def setup(self):
        from transformers import AutoTokenizer, AutoModelForSequenceClassification
        import torch

        self.tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL)
        self.model = AutoModelForSequenceClassification.from_pretrained(BASE_MODEL).to("cuda")
        self.model.eval()

    @modal.method()
    def predict(self, symptoms: str) -> dict:
        import torch
        import torch.nn.functional as F

        inputs = self.tokenizer(symptoms, return_tensors="pt", truncation=True).to("cuda")
        with torch.no_grad():
            logits = self.model(**inputs).logits
            probs = F.softmax(logits, dim=-1).squeeze().tolist()

        labels = self.model.config.id2label
        return {labels[i]: round(p, 3) for i, p in enumerate(probs)}
