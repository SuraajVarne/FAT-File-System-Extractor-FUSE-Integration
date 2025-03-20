from flask import Flask
from flask_graphql import GraphQLView
import graphene

class FileInfo(graphene.ObjectType):
    name = graphene.String()
    size = graphene.Int()
    created_at = graphene.String()

class Query(graphene.ObjectType):
    get_file_info = graphene.Field(FileInfo, name=graphene.String())

    def resolve_get_file_info(self, info, name):
        # Mocked data, integrate with FAT32 later
        return FileInfo(name=name, size=2048, created_at="2025-03-20")

schema = graphene.Schema(query=Query)

app = Flask(__name__)
app.add_url_rule("/graphql", view_func=GraphQLView.as_view("graphql", schema=schema, graphiql=True))

if __name__ == "__main__":
    app.run(debug=True)
