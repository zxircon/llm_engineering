import modal

VetBERTClassifier = modal.Cls.from_name("vetbert-service", "VetBERTClassifier")
classifier = VetBERTClassifier()

result = classifier.predict.remote("Kedimin ateşi var, sürekli kusuyor ve iştahsız.")
print(result)
