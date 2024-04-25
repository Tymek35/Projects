from django.db import models


class Score(models.Model):
    playerName = models.CharField(max_length=20)
    score = models.IntegerField()
    scoreDate = models.DateTimeField("score date", auto_now_add=True)
