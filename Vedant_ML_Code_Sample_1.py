
# coding: utf-8

# In[1]:


"""Example script that uses ML to break large paragraphs of text into keywords which are then used for sentiment analysis"""

import pandas as pd
import numpy as np
import os
from rake_nltk import Metric, Rake
from pathlib import Path
from sklearn.pipeline import Pipeline
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.linear_model import LogisticRegression
import spacy
from spacy.lang.en.stop_words import STOP_WORDS
from spacy.lang.en import English
from sklearn.feature_extraction.text import CountVectorizer,TfidfVectorizer
from sklearn.base import TransformerMixin
from sklearn.model_selection import train_test_split
from sklearn import metrics
import matplotlib.pyplot as plt

path = "Path"
data_folder = Path(path)
fileto = data_folder/"SOMEFILE"
df = pd.read_csv(fileto)
df = df.replace("#REF!",None)
df.dropna(inplace = True)
#load a dictionary
def load_words():
    fileto = data_folder/"words_alpha.txt"
    with open(fileto) as word_file:
        valid_words = set(word_file.read().split())
    return valid_words
english_words = load_words()

#scale linear features
for i in range(5):
    df["Label" + str(i)] = [int(100*number) for number in list(df["Label" + str(i)])]
    
#load spacy
punctuations = string.punctuation
nlp = spacy.load('en')
stop_words = spacy.lang.en.stop_words.STOP_WORDS
parser = English()

def tokenizer_(phrase):
    mytokens = parser(phrase)
    mytokens = [ word.lemma_.lower().strip() for word in tokens ]
    mytokens = [ word for word in tokens if word not in stop_words and word not in punctuations ]
    return mytokens
class predictors(TransformerMixin):
    def transform(self, X, **transform_params):
        return [text.strip().lower() for text in X]
    def fit(self, X, y=None, **fit_params):
        return self
    
m_vector = CountVectorizer(tokenizer = tokenizer_, ngram_range=(1,1))

#figures out the accuracy,precision, and recall scores for a basic logistic regression model on data
for a in df.columns:
    X = df['Label'] 
    ylabels = df[str(a)] 
    X_train, X_test, y_train, y_test = train_test_split(X, ylabels, test_size=0.3)
    classifier = LogisticRegression()
    pipe = Pipeline([("cleaner", predictors()),('vectorizer', m_vector),('classifier', classifier)])
    pipe.fit(X_train,y_train)
    predicted = pipe.predict(X_test)
    corr = 0
    tot = 0
    y_test2 = y_test.reset_index().drop(columns = ["index"])
    y_test2 = pd.Series(y_test2[str(a)])
    for i,v in y_test2.items():
        if(abs(y_test2[i] - predicted[i]) <= 50):
            corr+=1
        tot +=1
    print("Model Accuracy:",corr/tot)
    print("Model Precision:",metrics.precision_score(y_test, predicted, average = 'micro'))
    print("Model Recall:",metrics.recall_score(y_test, predicted, average = 'micro'))
    print()

