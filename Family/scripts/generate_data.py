#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
"寻根溯源"族谱管理系统 - 数据生成脚本
生成要求：
- 至少10个族谱
- 总计不少于100,000条成员数据
- 其中一个族谱拥有50,000以上的成员
- 单个族谱内至少30代人的传承关系
- 每个族谱内的成员至少与另一个成员有亲缘关系
"""

import random
import csv
import os
from datetime import datetime
from collections import defaultdict

random.seed(42)

SURNAMES = [
    '王', '李', '张', '刘', '陈', '杨', '赵', '黄', '周', '吴',
    '徐', '孙', '胡', '朱', '高', '林', '何', '郭', '马', '罗',
    '梁', '宋', '郑', '谢', '韩', '唐', '冯', '于', '董', '萧'
]

MALE_NAMES = [
    '伟', '芳', '娜', '秀', '敏', '静', '丽', '强', '磊', '军',
    '洋', '勇', '艳', '杰', '涛', '明', '超', '秀', '霞', '平',
    '刚', '桂', '英', '华', '建', '云', '海', '雪', '梅', '兰',
    '志', '文', '玉', '荣', '珍', '婷', '玲', '桂', '燕', '旭'
]

FEMALE_NAMES = [
    '芳', '娟', '英', '华', '慧', '巧', '美', '霞', '玲', '燕',
    '萍', '红', '兰', '玉', '珍', '贞', '莉', '兰', '凤', '洁',
    '梅', '琳', '素', '云', '莲', '真', '环', '雪', '荣', '妹',
    '霞', '月', '莺', '媛', '艳', '瑞', '凡', '佳', '嘉', '琼'
]

BIOGRAPHY_TEMPLATES = [
    "一生勤劳朴实，耕读传家。",
    "为人正直，乐于助人，在乡里享有崇高威望。",
    "学识渊博，教书育人，桃李满天下。",
    "精于农事，善于经营，家道日益兴旺。",
    "性情温和，与邻为善，家庭和睦。",
    "历经风雨，坚韧不拔，白手起家。",
    "精通医术，悬壶济世，救人无数。",
    "经商有道，诚信经营，富甲一方。",
    "投笔从戎，保家卫国，战功赫赫。",
    "潜心学问，著述丰富，为后世留下宝贵文化遗产。"
]

OUTPUT_DIR = "e:/Practice/QTpractice/Family/database/csv_data"


class FamilyTreeGenerator:
    def __init__(self, genealogy_id, genealogy_name, surname, target_size, min_generations):
        self.genealogy_id = genealogy_id
        self.genealogy_name = genealogy_name
        self.surname = surname
        self.target_size = target_size
        self.min_generations = min_generations

        self.persons = []
        self.families = []
        self.person_counter = 0
        self.family_counter = 0

        self.generations_data = defaultdict(list)
        self.birth_year = 1500

    def generate_name(self, gender):
        if gender == 'M':
            return self.surname + random.choice(MALE_NAMES)
        else:
            return self.surname + random.choice(FEMALE_NAMES)

    def create_person(self, name, gender, birth_year, death_year, generation, birth_family_id=None, biography=None):
        self.person_counter += 1
        person_id = self.person_counter

        if biography is None:
            biography = random.choice(BIOGRAPHY_TEMPLATES)

        person = {
            'person_id': person_id,
            'name': name,
            'gender': gender,
            'birth_year': birth_year,
            'death_year': death_year,
            'biography': biography,
            'generation': generation,
            'genealogy_id': self.genealogy_id,
            'birth_family_id': birth_family_id
        }
        self.persons.append(person)
        self.generations_data[generation].append(person_id)
        return person_id

    def create_family(self, husband_id, wife_id, marriage_year=None):
        self.family_counter += 1
        family_id = self.family_counter

        if marriage_year is None and husband_id:
            husband = next(p for p in self.persons if p['person_id'] == husband_id)
            marriage_year = husband['birth_year'] + random.randint(20, 30)

        family = {
            'family_id': family_id,
            'husband_id': husband_id,
            'wife_id': wife_id,
            'genealogy_id': self.genealogy_id,
            'marriage_year': marriage_year
        }
        self.families.append(family)
        return family_id

    def add_parent_child_relation(self, family_id, child_id):
        return {'family_id': family_id, 'child_id': child_id}

    def generate_founder(self):
        name = self.surname + random.choice(MALE_NAMES) if random.random() > 0.3 else self.surname + random.choice(FEMALE_NAMES)
        death_year = self.birth_year + random.randint(60, 90)
        return self.create_person(
            name=name,
            gender='M' if 'M' in name else 'F',
            birth_year=self.birth_year,
            death_year=death_year,
            generation=1
        )

    def generate_descendants(self, founder_id, current_generation, max_generation):
        if current_generation > max_generation:
            return

        founder = next(p for p in self.persons if p['person_id'] == founder_id)
        father_birth = founder['birth_year']

        num_children = random.randint(2, 5) if current_generation < 5 else random.randint(1, 4)

        children_ids = []
        for _ in range(num_children):
            if len(self.persons) >= self.target_size:
                return

            gender = random.choice(['M', 'F'])
            child_name = self.generate_name(gender)
            child_birth = father_birth + random.randint(20, 35)
            child_death = child_birth + random.randint(50, 85) if random.random() > 0.1 else None

            children_ids.append(self.create_person(
                name=child_name,
                gender=gender,
                birth_year=child_birth,
                death_year=child_death,
                generation=current_generation
            ))

        family_id = self.create_family(husband_id=founder_id if founder['gender'] == 'M' else None,
                                       wife_id=founder_id if founder['gender'] == 'F' else None)

        for child_id in children_ids:
            child = next(p for p in self.persons if p['person_id'] == child_id)
            child['birth_family_id'] = family_id

        for child_id in children_ids:
            self.generate_descendants(child_id, current_generation + 1, max_generation)

    def generate_large_genealogy(self):
        founder_id = self.generate_founder()
        print(f"  始祖 {founder_id} 已创建")

        current_gen = 2
        frontier = [founder_id]

        while len(self.persons) < self.target_size and current_gen <= 35:
            new_frontier = []
            persons_needed = self.target_size - len(self.persons)

            for parent_id in frontier:
                if len(self.persons) >= self.target_size:
                    break

                parent = next(p for p in self.persons if p['person_id'] == parent_id)

                spouse_name = self.generate_name('F' if parent['gender'] == 'M' else 'M')
                spouse_gender = 'F' if parent['gender'] == 'M' else 'M'
                spouse_birth = parent['birth_year'] + random.randint(-5, 5)

                spouse_id = self.create_person(
                    name=spouse_name,
                    gender=spouse_gender,
                    birth_year=spouse_birth,
                    death_year=spouse_birth + random.randint(50, 85) if random.random() > 0.05 else None,
                    generation=parent['generation']
                )

                family_id = self.create_family(
                    husband_id=parent_id if parent['gender'] == 'M' else spouse_id,
                    wife_id=spouse_id if parent['gender'] == 'M' else parent_id
                )

                children_count = random.randint(1, 4)
                for _ in range(children_count):
                    if len(self.persons) >= self.target_size:
                        break

                    child_gender = random.choice(['M', 'F'])
                    child_name = self.generate_name(child_gender)
                    child_birth = parent['birth_year'] + random.randint(20, 35)
                    child_death = child_birth + random.randint(50, 85) if random.random() > 0.15 else None

                    child_id = self.create_person(
                        name=child_name,
                        gender=child_gender,
                        birth_year=child_birth,
                        death_year=child_death,
                        generation=current_gen
                    )

                    child = next(p for p in self.persons if p['person_id'] == child_id)
                    child['birth_family_id'] = family_id

                    new_frontier.append(child_id)

            frontier = new_frontier
            current_gen += 1
            print(f"  第 {current_gen-1} 代完成，当前 {len(self.persons)} 人")

        print(f"  族谱 {self.genealogy_name} 生成完成，共 {len(self.persons)} 人")

    def generate_normal_genealogy(self):
        max_generation = random.randint(self.min_generations, 35)

        founder_id = self.generate_founder()
        print(f"  始祖 {founder_id} 已创建")

        self.generate_descendants(founder_id, 2, max_generation)

        while len(self.persons) < self.target_size:
            extra_root = random.choice(self.generations_data[random.randint(1, min(5, max_generation))])
            extra_person = next(p for p in self.persons if p['person_id'] == extra_root)
            self.generate_descendants(extra_root, extra_person['generation'] + 1, max_generation)

        print(f"  族谱 {self.genealogy_name} 生成完成，共 {len(self.persons)} 人")

    def generate(self):
        if self.target_size >= 40000:
            self.generate_large_genealogy()
        else:
            self.generate_normal_genealogy()

        return self.persons, self.families


class DataExporter:
    def __init__(self, output_dir):
        self.output_dir = output_dir
        os.makedirs(output_dir, exist_ok=True)

    def export_to_csv(self, genealogies_data, users_data, user_genealogy_data,
                      persons_data, families_data, parent_child_data):
        files = {
            'users.csv': users_data,
            'genealogies.csv': genealogies_data,
            'user_genealogy.csv': user_genealogy_data,
            'persons.csv': persons_data,
            'families.csv': families_data,
            'parent_child.csv': parent_child_data
        }

        for filename, data in files.items():
            filepath = os.path.join(self.output_dir, filename)
            if data:
                with open(filepath, 'w', newline='', encoding='utf-8') as f:
                    writer = csv.DictWriter(f, fieldnames=data[0].keys())
                    writer.writeheader()
                    writer.writerows(data)
                print(f"  导出: {filename} ({len(data)} 条记录)")


def generate_users(count=20):
    users = []
    for i in range(1, count + 1):
        users.append({
            'user_id': i,
            'username': f'user{i}',
            'password_hash': f'hash_{i}',
            'email': f'user{i}@example.com',
            'created_at': datetime.now().isoformat()
        })
    return users


def generate_genealogies_and_relations(genealogy_configs, users):
    genealogies = []
    user_genealogy = []
    all_persons = []
    all_families = []
    all_parent_child = []

    for idx, config in enumerate(genealogy_configs):
        print(f"\n正在生成族谱 {idx+1}/{len(genealogy_configs)}: {config['name']}")

        generator = FamilyTreeGenerator(
            genealogy_id=config['id'],
            genealogy_name=config['name'],
            surname=config['surname'],
            target_size=config['size'],
            min_generations=config['min_generations']
        )

        persons, families = generator.generate()
        all_persons.extend(persons)
        all_families.extend(families)

        genealogies.append({
            'genealogy_id': config['id'],
            'name': config['name'],
            'surname': config['surname'],
            'compile_time': f'{random.randint(1990, 2024)}-{random.randint(1, 12):02d}-{random.randint(1, 28):02d}',
            'description': f'{config["surname"]}氏族谱，详细记录了家族历代传承。',
            'creator_id': config['creator_id'],
            'created_at': datetime.now().isoformat()
        })

        user_genealogy.append({
            'user_id': config['creator_id'],
            'genealogy_id': config['id'],
            'role': 'creator',
            'joined_at': datetime.now().isoformat()
        })

        if config.get('editors'):
            for editor_id in config['editors']:
                user_genealogy.append({
                    'user_id': editor_id,
                    'genealogy_id': config['id'],
                    'role': 'editor',
                    'joined_at': datetime.now().isoformat()
                })

        family_id_mapping = {}
        for i, family in enumerate(families):
            family_id_mapping[i + 1] = family['family_id']

        persons_in_genealogy = [p for p in persons]
        for person in persons_in_genealogy:
            if person['birth_family_id']:
                original_id = person['birth_family_id']
                for i, fam in enumerate(families):
                    if fam['family_id'] == original_id:
                        person['birth_family_id'] = family_id_mapping.get(i + 1, original_id)
                        break

        persons_needing_parents = [p for p in persons_in_genealogy
                                    if p['birth_family_id'] and p['generation'] > 1]
        for person in persons_needing_parents:
            original_family_id = person['birth_family_id']
            for i, fam in enumerate(families):
                if fam['family_id'] == original_id:
                    new_family_id = family_id_mapping.get(i + 1)
                    if new_family_id:
                        parent_child_record = {
                            'family_id': new_family_id,
                            'child_id': person['person_id']
                        }
                        if parent_child_record not in all_parent_child:
                            all_parent_child.append(parent_child_record)
                    break

    return genealogies, user_genealogy, all_persons, all_families, all_parent_child


def generate_parent_child_relations(persons, families):
    parent_child = []
    person_family_map = defaultdict(list)

    for family in families:
        fid = family['family_id']
        if family['husband_id']:
            person_family_map[family['husband_id']].append(fid)
        if family['wife_id']:
            person_family_map[family['wife_id']].append(fid)

    for person in persons:
        if person['birth_family_id']:
            parent_child.append({
                'family_id': person['birth_family_id'],
                'child_id': person['person_id']
            })

    return parent_child


def main():
    print("=" * 60)
    print("族谱管理系统 - 模拟数据生成器")
    print("=" * 60)

    genealogy_configs = [
        {'id': 1, 'name': '李氏大宗谱', 'surname': '李', 'size': 55000, 'min_generations': 30, 'creator_id': 1, 'editors': [2, 3]},
        {'id': 2, 'name': '王氏族谱', 'surname': '王', 'size': 8000, 'min_generations': 20, 'creator_id': 2, 'editors': []},
        {'id': 3, 'name': '张氏族谱', 'surname': '张', 'size': 7500, 'min_generations': 18, 'creator_id': 3, 'editors': [1]},
        {'id': 4, 'name': '刘氏族谱', 'surname': '刘', 'size': 6000, 'min_generations': 15, 'creator_id': 4, 'editors': []},
        {'id': 5, 'name': '陈氏族谱', 'surname': '陈', 'size': 5500, 'min_generations': 14, 'creator_id': 5, 'editors': [2]},
        {'id': 6, 'name': '杨氏族谱', 'surname': '杨', 'size': 5000, 'min_generations': 13, 'creator_id': 6, 'editors': []},
        {'id': 7, 'name': '赵氏族谱', 'surname': '赵', 'size': 4500, 'min_generations': 12, 'creator_id': 7, 'editors': [1, 3]},
        {'id': 8, 'name': '黄氏族谱', 'surname': '黄', 'size': 4000, 'min_generations': 11, 'creator_id': 8, 'editors': []},
        {'id': 9, 'name': '周氏族谱', 'surname': '周', 'size': 3500, 'min_generations': 10, 'creator_id': 9, 'editors': [5]},
        {'id': 10, 'name': '吴氏族谱', 'surname': '吴', 'size': 3000, 'min_generations': 10, 'creator_id': 10, 'editors': []},
        {'id': 11, 'name': '徐氏族谱', 'surname': '徐', 'size': 2800, 'min_generations': 9, 'creator_id': 1, 'editors': [4]},
        {'id': 12, 'name': '孙氏族谱', 'surname': '孙', 'size': 2500, 'min_generations': 8, 'creator_id': 2, 'editors': []},
    ]

    total_target = sum(c['size'] for c in genealogy_configs)
    print(f"\n计划生成 {len(genealogy_configs)} 个族谱，共约 {total_target:,} 人")

    users = generate_users(20)
    print(f"\n生成用户数据: {len(users)} 个用户")

    genealogies, user_genealogy, persons, families, parent_child = \
        generate_genealogies_and_relations(genealogy_configs, users)

    print(f"\n生成关系数据:")
    print(f"  族谱: {len(genealogies)} 个")
    print(f"  成员: {len(persons)} 人")
    print(f"  家庭: {len(families)} 个")
    print(f"  亲子关系: {len(parent_child)} 条")
    print(f"  用户-族谱关系: {len(user_genealogy)} 条")

    exporter = DataExporter(OUTPUT_DIR)
    print(f"\n导出CSV文件到: {OUTPUT_DIR}")

    users_clean = []
    for u in users:
        u_copy = u.copy()
        del u_copy['email']
        users_clean.append(u_copy)

    genealogies_clean = []
    for g in genealogies:
        g_copy = g.copy()
        del g_copy['created_at']
        genealogies_clean.append(g_copy)

    persons_clean = []
    for p in persons:
        p_copy = p.copy()
        del p_copy['biography']
        persons_clean.append(p_copy)

    exporter.export_to_csv(
        genealogies_clean,
        users_clean,
        user_genealogy,
        persons_clean,
        families,
        parent_child
    )

    persons_bio_file = os.path.join(OUTPUT_DIR, 'persons_with_bio.csv')
    with open(persons_bio_file, 'w', newline='', encoding='utf-8') as f:
        if persons:
            writer = csv.DictWriter(f, fieldnames=persons[0].keys())
            writer.writeheader()
            writer.writerows(persons)
    print(f"  导出: persons_with_bio.csv (包含完整生平信息)")

    print("\n" + "=" * 60)
    print("数据生成完成!")
    print("=" * 60)


if __name__ == '__main__':
    main()
