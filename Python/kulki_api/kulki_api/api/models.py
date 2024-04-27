from django.db import models


class Score(models.Model):
    playerName = models.CharField(max_length=14)
    score = models.IntegerField()
    scoreDate = models.DateTimeField("score date", auto_now_add=True)
